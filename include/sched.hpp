#pragma once
#include "config.hpp"
#include "thread.hpp"

class Scheduler {
public:
    static void init();
    static void add_thread(ThreadControlBlock* thread);  // ✅ FIXED: match TCB type
    static void start();

    static void yield();                     // Manual context switch
    static void sleep(uint32_t ms);          // Sleep for ms milliseconds
    static void tick();                      // Called by SysTick

private:
    static ThreadControlBlock* threads[MAX_THREADS];
    static size_t thread_count;
    static ThreadControlBlock* current;
    static size_t current_index;

    static ThreadControlBlock* pick_next();
    static void switch_to(ThreadControlBlock* next);
};
