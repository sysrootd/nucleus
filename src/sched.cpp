#include "sched.hpp"
#include "stm32f401xe.h"
#include "thread.hpp"
#include <cstddef>

ThreadControlBlock* Scheduler::threads[MAX_THREADS] = {nullptr};
size_t Scheduler::thread_count = 0;
ThreadControlBlock* Scheduler::current = nullptr;
size_t Scheduler::current_index = 0;

extern "C" {
    void thread_launch(uint32_t* sp);
    void context_switch(uint32_t** old_sp, uint32_t* new_sp);

    ThreadControlBlock* scheduler_pick_next() { return Scheduler::pick_next(); }
}

void Scheduler::init() {
    for (auto &t : threads) t = nullptr;
    thread_count = 0;
    current = nullptr;
    current_index = 0;

    NVIC_SetPriority(PendSV_IRQn, 0xFF); // lowest priority
}

void Scheduler::add_thread(ThreadControlBlock* thread) {
    if (thread_count < MAX_THREADS) {
        thread->state = ThreadState::READY;
        thread->sleep_ticks = 0;
        threads[thread_count++] = thread;
    }
}

void Scheduler::start() {
    if (thread_count == 0) return;

    current = threads[0];
    current->state = ThreadState::RUNNING;

    // Set PSP to current thread’s stack pointer
    __set_PSP(reinterpret_cast<uint32_t>(current->sp));
    __set_CONTROL(0x03); // Use PSP, unprivileged
    __ISB();

    thread_launch(current->sp);

    while (true) __asm volatile("wfi");
}

void Scheduler::yield() {
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

void Scheduler::sleep(uint32_t ticks) {
    if (!current) return;
    current->sleep_ticks = ticks;
    current->state = ThreadState::SLEEPING;
    yield();
}

void Scheduler::tick() {
    for (size_t i = 0; i < thread_count; ++i) {
        auto* tcb = threads[i];
        if (tcb->state == ThreadState::SLEEPING && tcb->sleep_ticks > 0) {
            if (--tcb->sleep_ticks == 0) {
                tcb->state = ThreadState::READY;
            }
        }
    }
}

ThreadControlBlock* Scheduler::pick_next() {
    ThreadControlBlock* best = nullptr;
    uint8_t best_priority = 255;

    for (size_t i = 0; i < thread_count; ++i) {
        size_t idx = (current_index + i + 1) % thread_count;
        auto* tcb = threads[idx];
        if (tcb->state == ThreadState::READY && tcb->priority <= best_priority) {
            best_priority = tcb->priority;
            best = tcb;
            current_index = idx;
        }
    }
    return best ? best : current;
}

void Scheduler::switch_to(ThreadControlBlock* next) {
    if (!next || next == current) return;

    ThreadControlBlock* prev = current;
    prev->state = ThreadState::READY;
    current = next;
    current->state = ThreadState::RUNNING;

    context_switch(&prev->sp, current->sp);
}

extern "C" __attribute__((naked)) void PendSV_Handler(void) {
    __asm volatile(
        "mrs r0, psp              \n"
        "stmdb r0!, {r4-r11}      \n"
        "ldr r1, =_ZN9Scheduler7currentE \n"
        "ldr r2, [r1]             \n"
        "str r0, [r2]             \n"

        "bl scheduler_pick_next   \n"
        "ldr r1, =_ZN9Scheduler7currentE \n"
        "str r0, [r1]             \n"

        "ldr r2, [r0]             \n"
        "ldmia r2!, {r4-r11}      \n"
        "msr psp, r2              \n"
        "bx lr                    \n"
    );
}
