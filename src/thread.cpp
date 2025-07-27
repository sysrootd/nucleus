#include "thread.hpp"
#include "sched.hpp"
#include "stm32.hpp"
#include "stm32f401xe.h"
#include <cstdint>

[[noreturn]] void thread_exit_trap_impl(); // C++ version

extern "C" void thread_exit_trap() {
  thread_exit_trap_impl(); // delegate to C++-decorated version
}

[[noreturn]] void thread_exit_trap_impl() {
  while (true) {
    __asm volatile("nop");
  }
}

Thread::Thread(void (*entry)(void), uint8_t priority, uint32_t *stack_mem,
               size_t stack_size)
    : tcb() {
  tcb.priority = priority;
  tcb.state = ThreadState::READY;
  tcb.stack_base = stack_mem;
  tcb.stack_size = stack_size;
  tcb.sleep_ticks = 0;

  uint32_t *stack_top = stack_mem + stack_size;

  // Ensure 8-byte alignment
  stack_top = reinterpret_cast<uint32_t *>(
      reinterpret_cast<uintptr_t>(stack_top) & ~0x7);

  initialize_stack(entry, stack_top);
}

void Thread::initialize_stack(void (*entry)(void), uint32_t *stack_top) {
  uint32_t *sp = stack_top;

  uint32_t entry_val = reinterpret_cast<uint32_t>(entry);
  if (entry_val < 0x08000000 || entry_val >= 0x08080000 ||
      (entry_val & 1) == 0) {
    *((volatile uint32_t *)0x40020400) |= (1 << 28); // RED LED on
    while (true)
      ;
  }

  // Exception frame: hardware-pushed on exception return
  *(--sp) = 0x01000000;                                    // xPSR
  *(--sp) = reinterpret_cast<uint32_t>(entry) | 1U;        // PC
  *(--sp) = reinterpret_cast<uint32_t>(&thread_exit_trap); // LR
  *(--sp) = 0x12121212;                                    // R12
  *(--sp) = 0x03030303;                                    // R3
  *(--sp) = 0x02020202;                                    // R2
  *(--sp) = 0x01010101;                                    // R1
  *(--sp) = 0x00000000;                                    // R0

  // Software-saved registers
  *(--sp) = 0x08080808; // R11
  *(--sp) = 0x07070707; // R10
  *(--sp) = 0x06060606; // R9
  *(--sp) = 0x05050505; // R8
  *(--sp) = 0x04040404; // R7
  *(--sp) = 0x03030303; // R6
  *(--sp) = 0x02020202; // R5
  *(--sp) = 0x01010101; // R4

  // safety check
  if (sp == nullptr || sp < stack_top - 32 || sp >= stack_top) {
    *((volatile uint32_t *)0x40020400) |= (1 << 26);
    while (true)
      ;
  }

  tcb.sp = sp;
}

ThreadControlBlock *Thread::get_tcb() { return &tcb; }

extern "C" void thread_launch(uint32_t *sp) {
  __asm volatile("msr psp, r0         \n" // Set PSP = sp
                 "movs r0, #2         \n" // Use PSP, unprivileged
                 "msr control, r0     \n"
                 "isb                 \n"
                 "ldmia r0!, {r4-r11} \n" // Load from r0 (initial PSP)
                 "msr psp, r0         \n" // Update PSP to point past saved regs
                 "ldr lr, =0xFFFFFFFD \n" // Exception return using PSP
                 "bx lr               \n");
}

extern "C" void context_switch(uint32_t **old_sp, uint32_t *new_sp) {
  __asm volatile("mrs r2, psp          \n"
                 "stmdb r2!, {r4-r11}  \n"
                 "str r2, [%0]         \n"
                 "ldr r2, [%1]         \n"
                 "ldmia r2!, {r4-r11}  \n"
                 "msr psp, r2          \n"
                 "bx lr                \n"
                 :
                 : "r"(old_sp), "r"(new_sp)
                 : "r2", "memory");
}

extern "C" void HardFault_Handler(void) {
  // *((volatile uint32_t *)0x40020400) |= (1 << 26);
  while (true) {
    __asm volatile("nop");
  }
}
