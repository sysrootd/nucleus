#include "rtos.hpp"
#include "systick.hpp"      // systick_now()
#include "stm32.hpp"        // low-level init
#include "cmsis_gcc.h"      // __disable_irq / __enable_irq
#include "stm32f401xe.h"    // IRQn_Type, SCB defs
#include "core_cm4.h"       // Cortex-M4 core support

namespace rtos {

// --- Global state ---
TCB* g_current = nullptr;
TCB* g_ready[MAX_PRIOS] = { nullptr, nullptr, nullptr, nullptr };

static TCB g_threads[MAX_THREADS];
static uint32_t g_stacks[MAX_THREADS][STACK_WORDS];
static uint32_t g_thread_count = 0;

// --- Forward decl ---
static void schedule_ready(TCB* t);
static TCB* pick_next();

// --- Ready queue (circular linked list) ---
static void ready_push(TCB*& head, TCB* t) {
    if (!head) {
        head = t;
        t->next = t;
    } else {
        TCB* tail = head;
        while (tail->next != head) tail = tail->next;
        tail->next = t;
        t->next = head;
    }
}

static TCB* ready_pop(TCB*& head) {
    if (!head) return nullptr;
    TCB* t = head;
    if (t->next == t) {
        head = nullptr;
    } else {
        TCB* tail = head;
        while (tail->next != head) tail = tail->next;
        head = head->next;
        tail->next = head;
    }
    t->next = nullptr;
    return t;
}

static void ready_remove(TCB*& head, TCB* t) {
    if (!head) return;
    if (head == t && head->next == head) {
        head = nullptr;
        return;
    }
    if (head == t) {
        TCB* cur = head->next;
        while (cur->next != head) cur = cur->next;
        head = head->next;
        cur->next = head;
        return;
    }
    TCB* prev = head;
    TCB* cur = head->next;
    while (cur != head) {
        if (cur == t) {
            prev->next = cur->next;
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

// --- Scheduler ---
static void schedule_ready(TCB* t) {
    if (!t) return;
    t->state = State::Ready;
    ready_push(g_ready[t->priority], t);
}

static TCB* pick_next() {
    for (uint32_t p = 0; p < MAX_PRIOS; ++p) {
        if (g_ready[p]) {
            TCB* t = ready_pop(g_ready[p]);
            ready_push(g_ready[p], t); // round robin
            return t;
        }
    }
    return g_current; // nothing ready → stay on current
}

TCB* rtos_scheduler_next() {
    if (g_current && g_current->state == State::Ready) {
        schedule_ready(g_current);
    }
    TCB* n = pick_next();
    if (n) n->state = State::Ready;
    return n ? n : g_current;
}

// --- API ---
void init() {
    g_current = nullptr;
    for (uint32_t i = 0; i < MAX_PRIOS; ++i) g_ready[i] = nullptr;
    g_thread_count = 0;
}

int create_thread(void (*entry)(void*), void* arg, uint8_t priority) {
    if (g_thread_count >= MAX_THREADS) return -1;

    TCB* t = &g_threads[g_thread_count];
    uint32_t* stack = g_stacks[g_thread_count];

    // prepare initial stack frame (auto-restored on exception return)
    uint32_t* sp = stack + STACK_WORDS - 16;
    sp[8]  = (uint32_t)entry;   // PC
    sp[9]  = 0x01000000;        // xPSR (Thumb bit)
    sp[0]  = (uint32_t)arg;     // R0 = arg

    t->sp = sp;
    t->stack_base = stack;
    t->priority = priority;
    t->state = State::Ready;
    t->next = nullptr;
    t->wnext = nullptr;
    t->sleep_until = 0;

    schedule_ready(t);
    g_thread_count++;
    return g_thread_count - 1;
}

// external asm launcher
extern "C" void rtos_launch_first(void);

void start() {
    g_current = pick_next();
    if (!g_current) return;
    g_current->state = State::Ready;

    SysTick_Init();
    rtos_launch_first();   // never returns
}

void yield() {
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; // trigger PendSV
}

void sleep_ms(uint32_t ms) {
    g_current->state = State::Sleeping;
    g_current->sleep_until = systick_now() + ms;
    yield();
}

// --- Semaphore ---
void sem_init(Semaphore& s, int32_t initial) {
    s.count = initial;
    s.wait_list = nullptr;
}

void sem_post(Semaphore& s) {
    __disable_irq();
    s.count++;
    if (s.wait_list) {
        TCB* t = s.wait_list;
        s.wait_list = t->wnext;
        schedule_ready(t);
        s.count--;
    }
    __enable_irq();
}

void sem_wait(Semaphore& s) {
    __disable_irq();
    if (s.count > 0) {
        s.count--;
        __enable_irq();
        return;
    }
    g_current->state = State::Blocked;
    g_current->wnext = s.wait_list;
    s.wait_list = g_current;
    __enable_irq();
    yield();
}

} // namespace rtos

// --- PendSV + SysTick handlers ---
extern "C" void PendSV_Handler(void) {
    using namespace rtos;
    TCB* next = rtos_scheduler_next();
    if (next != g_current) {
        TCB* prev = g_current;
        g_current = next;
        asm volatile(
            "mrs r0, psp\n"
            "stmdb r0!, {r4-r11}\n"
            "str r0, [%0]\n"
            "ldr r0, [%1]\n"
            "ldmia r0!, {r4-r11}\n"
            "msr psp, r0\n"
            :
            : "r"(&prev->sp), "r"(&g_current->sp)
            : "r0", "memory"
        );
    }
}

extern "C" void rtos_tick_handler(void) {
    using namespace rtos;
    uint32_t now = systick_now();
    for (uint32_t i = 0; i < g_thread_count; ++i) {
        TCB* t = &g_threads[i];
        if (t->state == State::Sleeping && (int32_t)(t->sleep_until - now) <= 0) {
            schedule_ready(t);
        }
    }
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk; // request context switch
}
