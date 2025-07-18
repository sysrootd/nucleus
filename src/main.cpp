#include <cstddef>
#include "stm32.hpp"
#include "gpio.hpp"
#include "systick.hpp"
#include "sched.hpp"
#include "thread.hpp"

GPIO* gpioB = nullptr;

void led_task_1() {
    while (true) {
        gpioB->toggle(GPIOPin::Pin13);
        Scheduler::sleep(500);
    }
}

void led_task_2() {
    while (true) {
        gpioB->toggle(GPIOPin::Pin14);
        Scheduler::sleep(1000);
    }
}

constexpr size_t STACK_WORDS = 512;  // 512 * 4B = 2KB
alignas(8) static uint32_t stack1[STACK_WORDS];
alignas(8) static uint32_t stack2[STACK_WORDS];

int main() {
    static GPIO gpio(GPIOB);
    gpioB = &gpio;

    SysTick_Init();
    Scheduler::init();

    static Thread t1(led_task_1, 1, &stack1[STACK_WORDS - 1], STACK_WORDS);
    static Thread t2(led_task_2, 1, &stack2[STACK_WORDS - 1], STACK_WORDS);

    Scheduler::add_thread(t1.get_tcb());
    Scheduler::add_thread(t2.get_tcb());

    Scheduler::start();

    while (true);
}
