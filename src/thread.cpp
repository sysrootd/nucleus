#include "thread.hpp"
#include "sched.hpp"
#include "stm32.hpp"
#include <cstdint>

// Hard Fault debug information
struct HardFaultRegisters {
  uint32_t r0;
  uint32_t r1;
  uint32_t r2;
  uint32_t r3;
  uint32_t r12;
  uint32_t lr;
  uint32_t pc;
  uint32_t psr;
};

extern "C" {
uint32_t hardFaultRegisters[8]; // r0, r1, r2, r3, r12, lr, pc, psr
uint32_t hardFaultCfsr;
uint32_t hardFaultHfsr;
uint32_t hardFaultMmar;
uint32_t hardFaultBfar;
}

Thread::Thread(void (*entry)(void), uint8_t priority, uint32_t *stack_mem,
               size_t stack_size)
    : tcb() {
  // Initialize TCB
  tcb.priority = priority;
  tcb.state = ThreadState::READY;
  tcb.stack_base = stack_mem;
  tcb.stack_size = stack_size;
  tcb.sleep_ticks = 0;

  // Start at top of stack (full descending stack)
  uint32_t *sp = stack_mem + stack_size;

  // Ensure 8-byte alignment (Cortex-M requirement)
  sp = (uint32_t *)((uintptr_t)sp & ~0x7UL);

  // Additional registers that get saved during context switch (R4-R11)
  *(--sp) = 0x04040404; // R4
  *(--sp) = 0x05050505; // R5
  *(--sp) = 0x06060606; // R6
  *(--sp) = 0x07070707; // R7
  *(--sp) = 0x08080808; // R8
  *(--sp) = 0x09090909; // R9
  *(--sp) = 0x10101010; // R10
  *(--sp) = 0x11111111; // R11

  // Exception stack frame (auto-pushed by hardware)
  *(--sp) = 0x00000000;            // R0
  *(--sp) = 0x01010101;            // R1
  *(--sp) = 0x02020202;            // R2
  *(--sp) = 0x03030303;            // R3
  *(--sp) = 0x12121212;            // R12
  *(--sp) = 0xFFFFFFFD;            // LR (EXC_RETURN: thread mode, PSP, no FPU)
  *(--sp) = ((uint32_t)entry) | 1; // PC (entry point with Thumb bit set)
  *(--sp) = 0x01000000; // xPSR (Thumb bit set, no other special flags)

  tcb.sp = sp; // Set initial stack pointer
}

ThreadControlBlock *Thread::get_tcb() { return &tcb; }

// Enhanced Hard Fault Handler
extern "C" __attribute__((naked)) void HardFault_Handler(void) {
  __asm volatile(
      "tst lr, #4              \n" // Check EXC_RETURN to determine stack
                                   // pointer
      "ite eq                  \n"
      "mrseq r0, msp           \n" // If zero, use MSP
      "mrsne r0, psp           \n" // Else use PSP
      "ldr r1, =hardFaultRegisters \n"
      "stmia r1!, {r4-r11}     \n" // Save remaining registers
      "ldr r2, [r0, #0x00]     \n" // Load R0 from stack
      "str r2, [r1, #0x00]     \n"
      "ldr r2, [r0, #0x04]     \n" // R1
      "str r2, [r1, #0x04]     \n"
      "ldr r2, [r0, #0x08]     \n" // R2
      "str r2, [r1, #0x08]     \n"
      "ldr r2, [r0, #0x0C]     \n" // R3
      "str r2, [r1, #0x0C]     \n"
      "ldr r2, [r0, #0x10]     \n" // R12
      "str r2, [r1, #0x10]     \n"
      "ldr r2, [r0, #0x14]     \n" // LR
      "str r2, [r1, #0x14]     \n"
      "ldr r2, [r0, #0x18]     \n" // PC
      "str r2, [r1, #0x18]     \n"
      "ldr r2, [r0, #0x1C]     \n" // PSR
      "str r2, [r1, #0x1C]     \n"

      // Capture fault status registers
      "ldr r0, =0xE000ED28     \n" // CFSR address
      "ldr r1, [r0]            \n"
      "ldr r2, =hardFaultCfsr  \n"
      "str r1, [r2]            \n"

      "ldr r0, =0xE000ED2C     \n" // HFSR address
      "ldr r1, [r0]            \n"
      "ldr r2, =hardFaultHfsr  \n"
      "str r1, [r2]            \n"

      "ldr r0, =0xE000ED34     \n" // MMFAR address
      "ldr r1, [r0]            \n"
      "ldr r2, =hardFaultMmar  \n"
      "str r1, [r2]            \n"

      "ldr r0, =0xE000ED38     \n" // BFAR address
      "ldr r1, [r0]            \n"
      "ldr r2, =hardFaultBfar  \n"
      "str r1, [r2]            \n"

      // Signal error (turn on LED)
      "ldr r0, =0x40020400     \n" // GPIOB base (adjust for your MCU)
      "ldr r1, [r0, #0x14]     \n" // ODR offset
      "orr r1, r1, #(1 << 13)  \n" // PB13
      "str r1, [r0, #0x14]     \n"

      // Infinite loop
      "b .                     \n");
}

// Assembly implementations (should be in a separate .S file, but included here
// for completeness)
extern "C" {
void thread_launch(uint32_t *sp) {
  __asm volatile(
      "msr psp, %0         \n" // Set process stack pointer
      "mov r0, #3          \n" // Switch to unprivileged thread mode with PSP
      "msr control, r0     \n"
      "isb                  \n" // Instruction sync barrier

      // Pop the registers we pushed in Thread constructor
      "pop {r4-r11}         \n" // Restore R4-R11

      // The following will be automatically popped by exception return:
      // r0-r3, r12, lr, pc, psr

      "bx lr                \n" // Should never reach here
      :
      : "r"(sp));
}

void context_switch(uint32_t **old_sp, uint32_t *new_sp) {
  __asm volatile(
      // Save current context (R4-R11)
      "mrs r2, psp          \n" // Get current PSP
      "stmdb r2!, {r4-r11}  \n" // Save remaining regs
      "str r2, [%0]         \n" // Save old SP

      // Restore new context
      "ldr r2, [%1]         \n" // Get new SP
      "ldmia r2!, {r4-r11}  \n" // Restore regs
      "msr psp, r2          \n" // Update PSP

      // Return (will use the PC/LR/PSR from the new stack)
      "bx lr                \n"
      :
      : "r"(old_sp), "r"(new_sp)
      : "r2");
}
}