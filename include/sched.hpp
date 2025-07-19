#pragma once

#include "config.hpp"
#include "tcb.hpp"
#include "thread.hpp"
#include <cstddef>

class Scheduler {
public:
  // Initialization and control
  static void init(); // Initialize scheduler
  static void
  add_thread(ThreadControlBlock *thread); // Add a thread to scheduling
  static void start();                    // Start the scheduler

  // Thread control
  static void yield();            // Voluntarily yield CPU
  static void sleep(uint32_t ms); // Put current thread to sleep
  static void tick();             // System tick handler

  // Thread information (for internal use)
  static ThreadControlBlock *get_current() { return current; }
  static size_t get_thread_count() { return thread_count; }
  // Scheduling algorithm
  static ThreadControlBlock *pick_next();          // Select next thread to run
  static void switch_to(ThreadControlBlock *next); // Perform context switch

  // Thread management
  static constexpr size_t MAX_THREADS = DEFAULT_MAX_THREADS; // From config
  static ThreadControlBlock *threads[MAX_THREADS]; // Thread pointer array
  static size_t thread_count;                      // Active thread count
  static ThreadControlBlock *current;              // Currently running thread
  static size_t current_index;                     // Index of current thread

  // Disallow instantiation
  Scheduler() = delete;
  Scheduler(const Scheduler &) = delete;
  Scheduler &operator=(const Scheduler &) = delete;
};