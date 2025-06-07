#include "thread.hpp"

// Constructor
Thread::Thread(void (*entry)(void), uint8_t priority) : tcb{}, stack{} {
    tcb.priority = priority;
    tcb.state = ThreadState::READY;
    tcb.sleep_ticks = 0;
    tcb.stack_base = &stack[0];

    setup_stack(entry);
}

// Setup initial stack frame for Cortex-M context switch (via PendSV)
void Thread::setup_stack(void (*entry)(void)) {
    uint32_t* sp = &stack[STACK_SIZE];  // Start from top of stack

    // Hardware-stacked registers (automatically stacked on exception entry)
    *(--sp) = 0x01000000;        // xPSR (Thumb bit set)
    *(--sp) = (uint32_t)entry;   // PC
    *(--sp) = 0xFFFFFFFD;        // LR (return to Thread mode, use PSP)
    *(--sp) = 0; // R12
    *(--sp) = 0; // R3
    *(--sp) = 0; // R2
    *(--sp) = 0; // R1
    *(--sp) = 0; // R0

    // Software-saved registers (saved/restored manually in PendSV)
    *(--sp) = 0; // R11
    *(--sp) = 0; // R10
    *(--sp) = 0; // R9
    *(--sp) = 0; // R8
    *(--sp) = 0; // R7
    *(--sp) = 0; // R6
    *(--sp) = 0; // R5
    *(--sp) = 0; // R4

    // Final stack pointer for this thread
    tcb.sp = sp;
}

// Return pointer to the thread's control block
ThreadControlBlock* Thread::get_tcb() {
    return &tcb;
}
