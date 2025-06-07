#pragma once

#include <cstdint>
#include <cstddef>

constexpr size_t STACK_SIZE = 128;

enum class ThreadState {
    READY,
    RUNNING,
    BLOCKED,
    SLEEPING
};

struct ThreadControlBlock {
    uint32_t* sp;               // Stack pointer
    uint8_t priority;           // Priority
    ThreadState state;          // Current state
    uint32_t sleep_ticks;       // Sleep counter
    uint32_t* stack_base;       // Base address of the stack
};

class Thread {
public:
    Thread(void (*entry)(void), uint8_t priority);
    ThreadControlBlock* get_tcb();

private:
    ThreadControlBlock tcb;
    alignas(8) uint32_t stack[STACK_SIZE];

    void setup_stack(void (*entry)(void));
};
