#include "thread.hpp"
#include <cstdint>

// Forward declaration for thread exit trap
extern "C" [[noreturn]] void thread_exit_trap();

[[noreturn]] void thread_exit_trap_impl() {
  while (true) {
    __asm volatile("bkpt #0"); // Trap if a thread returns
  }
}

extern "C" [[noreturn]] void thread_exit_trap() { thread_exit_trap_impl(); }

Thread::Thread(void (*entry)(void), uint8_t priority, uint32_t *stack_mem,
               size_t stack_size)
    : tcb() {
  tcb.priority = priority;
  tcb.state = ThreadState::READY;
  tcb.stack_base = stack_mem;
  tcb.stack_size = stack_size;
  tcb.sleep_ticks = 0;

  uint32_t *stack_top = stack_mem + stack_size;
  stack_top = reinterpret_cast<uint32_t *>(
      reinterpret_cast<uintptr_t>(stack_top) & ~0x7); // align

  initialize_stack(entry, stack_top);
}

void Thread::initialize_stack(void (*entry)(void), uint32_t *stack_top) {
  uint32_t *sp = stack_top;

  // Hardware-saved context (8 words)
  *(--sp) = 0x01000000;                                    // xPSR
  *(--sp) = reinterpret_cast<uint32_t>(entry) | 1U;        // PC
  *(--sp) = reinterpret_cast<uint32_t>(&thread_exit_trap); // LR
  *(--sp) = 0x12121212;                                    // R12
  *(--sp) = 0x03030303;                                    // R3
  *(--sp) = 0x02020202;                                    // R2
  *(--sp) = 0x01010101;                                    // R1
  *(--sp) = 0x00000000;                                    // R0

  // Software-saved context (callee-saved regs)
  *(--sp) = 0x11111111; // R11
  *(--sp) = 0x10101010; // R10
  *(--sp) = 0x09090909; // R9
  *(--sp) = 0x08080808; // R8
  *(--sp) = 0x07070707; // R7
  *(--sp) = 0x06060606; // R6
  *(--sp) = 0x05050505; // R5
  *(--sp) = 0x04040404; // R4

  tcb.sp = sp;
}

ThreadControlBlock *Thread::get_tcb() { return &tcb; }

extern "C" void HardFault_Handler(void) {
  while (true) {
    __asm volatile("bkpt #1");
  }
}
