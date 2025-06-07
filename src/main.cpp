//-----------------------include files----------------------//
#include "stm32.hpp"
#include "gpio.hpp"
#include "adc.hpp"
#include "uart.hpp"
#include "stm32f401xe.h"
#include "systick.hpp"
#include "sched.hpp"
#include "thread.hpp"
#include "gpio.hpp"

GPIO gpioB = GPIO(GPIOB);  // Create instance for GPIOA

void led_task_1() {
    while (true) {
        gpioB.toggle(GPIOPin::Pin13);
        Scheduler::sleep(500);
    }
}

void led_task_2() {
    while (true) {
        gpioB.toggle(GPIOPin::Pin14);
        Scheduler::sleep(1000);
    }
}

int main() {
    // Enable GPIOB clock
    RCC->AHB1ENR |= (0x01<<1);

    // Configure PA5 and PA6 as output
    gpioB.set_mode(GPIOPin::Pin13, GPIOMode::Output);
    gpioB.set_mode(GPIOPin::Pin14, GPIOMode::Output);

    // Initialize SysTick for 1ms tick
    SysTick_Init();

    // Initialize scheduler
    Scheduler::init();

    // Configure PC0 (ADC Channel 10) as analog input
    GPIO gpioC = GPIO(GPIOC);
    gpioC.set_mode(GPIOPin::Pin0, GPIOMode::Analog);
    // Create threads
    static Thread t1 = Thread(led_task_1, 1);
    static Thread t2 = Thread(led_task_2, 1);

    // Add threads to scheduler
    Scheduler::add_thread(t1.get_tcb());
    Scheduler::add_thread(t2.get_tcb());

    // Start the scheduler
    Scheduler::start();

    while (true);  // Should never be reached
}
