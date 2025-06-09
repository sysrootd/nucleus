#pragma once

#include <cstdint>

enum class ThreadState {
    READY,
    RUNNING,
    SLEEPING,
    BLOCKED
};

struct ThreadControlBlock {
    uint32_t* sp;            // Stack pointer (current top of stack)
    uint32_t* stack_base;    // Stack base (bottom of stack memory)
    uint8_t priority;        // Thread priority
    ThreadState state;       // Thread state
    uint32_t sleep_ticks;    // For sleep/delay tracking
};
