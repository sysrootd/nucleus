#pragma once

#include <cstddef>
#include <cstdint>

// Thread states
enum class ThreadState : uint8_t {
  READY,     // Thread is ready to run
  RUNNING,   // Thread is currently running
  SLEEPING,  // Thread is sleeping/waiting
  SUSPENDED, // Thread is suspended (not scheduled)
  TERMINATED // Thread has finished execution
};

// Thread Control Block structure
struct ThreadControlBlock {
  uint32_t *sp;         // Current stack pointer
  uint8_t priority;     // Thread priority (0 = highest)
  ThreadState state;    // Current thread state
  uint32_t *stack_base; // Base address of stack
  size_t stack_size;    // Stack size in words (NOT bytes)
  uint32_t sleep_ticks; // Remaining sleep ticks
#ifdef THREAD_DEBUG
  const char *name;
  uint32_t runtime;
#endif
};

// Static assertions to ensure proper sizing
static_assert(sizeof(ThreadState) == 1, "ThreadState should be 1 byte");
static_assert(sizeof(ThreadControlBlock) <= 32, "TCB size may be too large");