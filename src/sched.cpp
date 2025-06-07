#include "sched.hpp"
#include "thread.hpp"
#include <algorithm>

ThreadControlBlock* Scheduler::threads[MAX_THREADS] = {nullptr};
size_t Scheduler::thread_count = 0;
ThreadControlBlock* Scheduler::current = nullptr;
size_t Scheduler::current_index = 0;

void Scheduler::init() {
    thread_count = 0;
    current = nullptr;
    current_index = 0;
}

void Scheduler::add_thread(ThreadControlBlock* thread) {
    if (thread_count < MAX_THREADS) {
        threads[thread_count++] = thread;
    }
}

void Scheduler::start() {
    if (thread_count == 0) return;

    // Pick highest priority READY thread
    current_index = 0;
    current = pick_next();

    if (!current) return;

    current->state = ThreadState::RUNNING;

    // Set PSP to current thread SP
    __asm volatile(
        "msr psp, %0\n"
        "movs r0, #2\n"
        "msr control, r0\n"
        "isb\n"
        :
        : "r"(current->sp)
        : "memory"
    );

    // Start first thread (simulate exception return)
    __asm volatile(
        "mov lr, #0xFFFFFFFD\n"
        "bx lr\n"
    );
}

void Scheduler::yield() {
    // Called to voluntarily yield CPU to next thread
    ThreadControlBlock* old = current;

    current_index = (current_index + 1) % thread_count;
    current = pick_next();

    if (current == old) return; // no other ready thread

    switch_to(current);
}

void Scheduler::sleep(uint32_t ms) {
    if (!current) return;

    current->sleep_ticks = ms;
    current->state = ThreadState::SLEEPING;

    yield();
}

void Scheduler::tick() {
    // Called every SysTick interrupt
    for (size_t i = 0; i < thread_count; ++i) {
        ThreadControlBlock* tcb = threads[i];
        if (tcb->state == ThreadState::SLEEPING && tcb->sleep_ticks > 0) {
            tcb->sleep_ticks--;
            if (tcb->sleep_ticks == 0) {
                tcb->state = ThreadState::READY;
            }
        }
    }
}

ThreadControlBlock* Scheduler::pick_next() {
    ThreadControlBlock* best = nullptr;
    uint8_t best_priority = 255;

    for (size_t i = 0; i < thread_count; ++i) {
        ThreadControlBlock* tcb = threads[i];
        if ((tcb->state == ThreadState::READY || tcb->state == ThreadState::RUNNING) && tcb->priority < best_priority) {
            best_priority = tcb->priority;
            best = tcb;
            current_index = i;
        }
    }
    return best;
}

extern "C" void context_switch(uint32_t** old_sp, uint32_t* new_sp);

void Scheduler::switch_to(ThreadControlBlock* next) {
    ThreadControlBlock* prev = current;
    current = next;

    if (prev == current) return;

    prev->state = ThreadState::READY;
    current->state = ThreadState::RUNNING;

    context_switch(&prev->sp, current->sp);
}
