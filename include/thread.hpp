#pragma once

#include <cstdint>
#include "sched.hpp"
#include "tcb.hpp"
#include <cstddef>

class Thread {
public:
    // Constructor with essential thread parameters
    Thread(void (*entry)(void), uint8_t priority, uint32_t* stack_mem, size_t stack_size);
    
    // Get the thread's control block
    ThreadControlBlock* get_tcb();
    
    // Disallow copying and assignment
    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;

private:
    ThreadControlBlock tcb;
    
    // Helper function for stack initialization
    void initialize_stack(void (*entry)(void), uint32_t* stack_top);
    
    // Constants for stack initialization
    static constexpr uint32_t kExcReturnThreadPsp = 0xFFFFFFFD;
    static constexpr uint32_t INITIAL_XPSR = 0x01000000; // Thumb state
};
    void setup_stack(void (*entry)(void), uint32_t* stack_top);
};
