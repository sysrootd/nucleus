#include "thread.hpp"
#include "stm32.hpp"

Thread::Thread(void (*entry)(void), uint8_t priority, uint32_t* stack_mem, size_t stack_size) {
    tcb.priority = priority;
    tcb.state = ThreadState::READY;
    tcb.stack_base = stack_mem;
    tcb.sleep_ticks = 0;

    // Top of stack (full descending stack)
    uint32_t* sp = stack_mem + stack_size;

    // Ensure 8-byte alignment
    sp = (uint32_t*)((uintptr_t)sp & ~0x7);

    // Simulated exception stack frame
    *(--sp) = 0x01000000;         // xPSR (Thumb bit set)
    *(--sp) = ((uint32_t)entry) | 1;    // PC (entry point)
    *(--sp) = 0xFFFFFFFD;         // LR (return with PSP)
    *(--sp) = 0x12121212;         // R12
    *(--sp) = 0x03030303;         // R3
    *(--sp) = 0x02020202;         // R2
    *(--sp) = 0x01010101;         // R1
    *(--sp) = 0x00000000;         // R0

    // Additional registers that get saved during context switch
    *(--sp) = 0x11111111; // R11
    *(--sp) = 0x10101010; // R10
    *(--sp) = 0x09090909; // R9
    *(--sp) = 0x08080808; // R8
    *(--sp) = 0x07070707; // R7
    *(--sp) = 0x06060606; // R6
    *(--sp) = 0x05050505; // R5
    *(--sp) = 0x04040404; // R4

    tcb.sp = sp;  // Set initial SP
}

ThreadControlBlock* Thread::get_tcb() {
    return &tcb;
}


extern "C" void HardFault_Handler(void) {
    GPIOB->ODR |= (0x01<<13);
}