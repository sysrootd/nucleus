#include "thread.hpp"
#include "sched.hpp"
#include "stm32.hpp"
#include "stm32f401xe.h"
#include <cstdint>

// Trap for threads that return
extern "C" void thread_exit_trap();

Thread::Thread(void (*entry)(void), uint8_t priority, uint32_t *stack_mem,
               size_t stack_size)
    : tcb() {
  tcb.priority = priority;
  tcb.state = ThreadState::READY;
  tcb.stack_base = stack_mem;
  tcb.stack_size = stack_size;
  tcb.sleep_ticks = 0;

  // Top of the stack (stack grows down)
  uint32_t *stack_top = stack_mem + stack_size;

  // Align to 8 bytes
  stack_top = reinterpret_cast<uint32_t *>(
      reinterpret_cast<uintptr_t>(stack_top) & ~0x7);

  initialize_stack(entry, stack_top);
}

void Thread::initialize_stack(void (*entry)(void), uint32_t *stack_top) {
  uint32_t *sp = stack_top;

  uint32_t entry_val = reinterpret_cast<uint32_t>(entry);
  if (entry_val < 0x08000000 || entry_val >= 0x08080000 ||
      (entry_val & 1) == 0) {
    *((volatile uint32_t *)0x40020400) |= (1 << 28); // RED LED
    while (true)
      ;
  }

  // Hardware-saved registers on exception return
  *(--sp) = 0x01000000; // xPSR (Thumb bit set)
  *(--sp) =
      reinterpret_cast<uint32_t>(entry) | 1U; // PC (entry point + thumb bit)
  *(--sp) = reinterpret_cast<uint32_t>(&thread_exit_trap); // LR
  *(--sp) = 0x12121212;                                    // R12
  *(--sp) = 0x03030303;                                    // R3
  *(--sp) = 0x02020202;                                    // R2
  *(--sp) = 0x01010101;                                    // R1
  *(--sp) = 0x00000000;                                    // R0

  // Software-saved registers (R4–R11)
  *(--sp) = 0x08080808; // R11
  *(--sp) = 0x07070707; // R10
  *(--sp) = 0x06060606; // R9
  *(--sp) = 0x05050505; // R8
  *(--sp) = 0x04040404; // R7
  *(--sp) = 0x03030303; // R6
  *(--sp) = 0x02020202; // R5
  *(--sp) = 0x01010101; // R4

  tcb.sp = sp;
}

ThreadControlBlock *Thread::get_tcb() { return &tcb; }

extern "C" void thread_exit_trap() {
  while (true)
    ;
}

extern "C" void thread_launch(uint32_t *sp) {
  __asm volatile("msr psp, r0         \n"
                 "movs r0, #1         \n" // CONTROL = 1 → PSP + privileged
                 "msr control, r0     \n"
                 "isb                 \n"
                 "pop {r4-r11}        \n"
                 "ldr r0, =0x40020400     \n"
                 "ldr r1, [r0]            \n"
                 "orr r1, r1, #(1 << 26)  \n" // turn on RED
                 "str r1, [r0]            \n"
                 "ldr lr, =0xFFFFFFFD \n"
                 "bx lr               \n");
}

// Context switch between threads
extern "C" void context_switch(uint32_t **old_sp, uint32_t *new_sp) {
  __asm volatile("mrs r2, psp          \n" // Get current PSP
                 "stmdb r2!, {r4-r11}  \n" // Save software context
                 "str r2, [%0]         \n" // Save updated SP

                 "ldr r2, [%1]         \n" // Load new SP
                 "ldmia r2!, {r4-r11}  \n" // Restore new context
                 "msr psp, r2          \n" // Update PSP
                 "bx lr                \n"
                 :
                 : "r"(old_sp), "r"(new_sp)
                 : "r2", "memory");
}

extern "C" void HardFault_Handler(void) {
  while (true) {
    __asm volatile("nop");
  }
}
