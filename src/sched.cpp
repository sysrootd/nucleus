#include "sched.hpp"
#include "stm32.hpp"
#include "stm32f401xe.h"
#include "thread.hpp"
#include <cstddef>

// Initialize static members
ThreadControlBlock *Scheduler::threads[MAX_THREADS] = {nullptr};
size_t Scheduler::thread_count = 0;
ThreadControlBlock *Scheduler::current = nullptr;
size_t Scheduler::current_index = 0;

// Assembly function declarations
extern "C" {
void thread_launch(uint32_t *sp);
void context_switch(uint32_t **old_sp, uint32_t *new_sp);
}

void Scheduler::init() {
  // Reset all scheduler state
  for (size_t i = 0; i < MAX_THREADS; i++) {
    threads[i] = nullptr;
  }
  thread_count = 0;
  current = nullptr;
  current_index = 0;

  // Configure PendSV to lowest priority
  NVIC_SetPriority(PendSV_IRQn, 0xFF);
}

void Scheduler::add_thread(ThreadControlBlock *thread) {
  if (thread_count >= MAX_THREADS)
    return;

  // Initialize thread state
  thread->state = ThreadState::READY;
  thread->sleep_ticks = 0;

  threads[thread_count++] = thread;
}

void Scheduler::start() {
  if (thread_count == 0)
    return;

  current = threads[0];
  current->state = ThreadState::RUNNING;

  // Start the first thread directly
  thread_launch(current->sp);

  // Execution will continue in PendSV_Handler
  while (true) {
    __asm volatile("wfi");
  }
}

void Scheduler::yield() {
  ThreadControlBlock *old = current;
  current = pick_next();

  if (current == old)
    return;

  switch_to(current);
}

void Scheduler::sleep(uint32_t ms) {
  if (!current)
    return;

  current->sleep_ticks = ms;
  current->state = ThreadState::SLEEPING;

  yield();
}

void Scheduler::tick() {
  for (size_t i = 0; i < thread_count; ++i) {
    ThreadControlBlock *tcb = threads[i];
    if (tcb->state == ThreadState::SLEEPING && tcb->sleep_ticks > 0) {
      if (--tcb->sleep_ticks == 0) {
        tcb->state = ThreadState::READY;
      }
    }
  }
}

ThreadControlBlock *Scheduler::pick_next() {
  ThreadControlBlock *best = nullptr;
  uint8_t best_priority = 255; // Lower number = higher priority

  for (size_t i = 0; i < thread_count; ++i) {
    size_t idx =
        (current_index + i + 1) % thread_count; // Round-robin within priorities
    ThreadControlBlock *tcb = threads[idx];

    if (tcb->state == ThreadState::READY && tcb->priority <= best_priority) {
      best_priority = tcb->priority;
      best = tcb;
      current_index = idx;
    }
  }

  return best ? best : current; // Fallback to current if none found
}

void Scheduler::switch_to(ThreadControlBlock *next) {
  if (!next || next == current)
    return;

  ThreadControlBlock *prev = current;
  prev->state = ThreadState::READY;

  current = next;
  current->state = ThreadState::RUNNING;

  // Perform the actual context switch
  context_switch(&prev->sp, current->sp);
}

// PendSV Handler - must be naked to prevent unwanted prologue/epilogue
extern "C" __attribute__((naked)) void PendSV_Handler(void) {
  __asm volatile(
      "cpsid i                 \n" // Disable interrupts
      "mrs r0, psp             \n" // Get current thread's stack pointer

      // Save current context (R4-R11)
      "stmdb r0!, {r4-r11}     \n"

      // Store current SP
      "ldr r2, =_ZN9Scheduler7currentE \n"
      "ldr r1, [r2]            \n"
      "str r0, [r1]            \n"

      // Load next thread's SP
      "ldr r1, [r2]            \n" // Reload current (now points to next)
      "ldr r0, [r1]            \n" // Get next thread's SP

      // Restore next context
      "ldmia r0!, {r4-r11}     \n"

      // Update PSP
      "msr psp, r0              \n"

      // Enable interrupts and return
      "cpsie i                 \n"
      "bx lr                   \n");
}