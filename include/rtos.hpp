#pragma once
#include "stm32.hpp"
#include "systick.hpp"
#include "stm32f401xe.h"   // device header, defines IRQn_Type & __NVIC_PRIO_BITS
#include "core_cm4.h" 

extern "C" void PendSV_Handler(void);
extern "C" void SysTick_Handler(void);
extern "C" void rtos_tick_handler(void);
extern "C" void rtos_launch_first(void);

namespace rtos {

// --- Config ---
constexpr uint32_t MAX_THREADS   = 8;
constexpr uint32_t STACK_WORDS   = 256;  // per-thread stack size (words)
constexpr uint32_t TICK_HZ       = 1000; // 1ms tick
constexpr uint32_t MAX_PRIOS     = 4;

// --- Types ---
enum class State : uint8_t { Ready, Sleeping, Blocked };

struct TCB;

struct Semaphore {
    volatile int32_t count;
    TCB* wait_list;
};

struct TCB {
    uint32_t* sp;
    uint32_t* stack_base;
    State state;
    uint8_t priority;
    TCB* next;
    TCB* wnext;
    volatile uint32_t sleep_until;
};

// --- Globals (declared here, defined in rtos.cpp) ---
extern TCB* g_current;
extern TCB* g_ready[MAX_PRIOS];

// --- API ---
void init();
int  create_thread(void (*entry)(void*), void* arg, uint8_t priority=1);
void start();
void yield();
void sleep_ms(uint32_t ms);

// --- Semaphores ---
void sem_init(Semaphore& s, int32_t initial);
void sem_post(Semaphore& s);
void sem_wait(Semaphore& s);

} // namespace rtos
