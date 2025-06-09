#pragma once

#include <cstdint>
#include "sched.hpp"
#include "tcb.hpp"
#include <cstddef>

class Thread {
public:
    Thread(void (*entry)(void), uint8_t priority, uint32_t* stack_mem, size_t stack_size);
    ThreadControlBlock* get_tcb();

private:
    ThreadControlBlock tcb;
    void setup_stack(void (*entry)(void), uint32_t* stack_top);
};
