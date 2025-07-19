#include "thread.hpp"
#include "sched.hpp"
#include "stm32.hpp"
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
  while (true) {
    // Trap if thread exits unexpectedly
  }
}

// First-time launch into thread
extern "C" void thread_launch(uint32_t *sp) {
  // *((volatile uint32_t*)0x40020400) |= (1 << 26);
  __asm volatile("msr psp, r0         \n" // Load Process Stack Pointer
                 "movs r0, #2         \n" // CONTROL = 2 (use PSP, unprivileged)
                 "msr control, r0     \n"
                 "isb                 \n"

                 "pop {r4-r11}        \n" // Restore software-saved context

                 "ldr lr, =0xFFFFFFFD \n" // Return to Thread mode using PSP
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