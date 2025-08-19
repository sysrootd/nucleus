#include "gpio.hpp"
#include "sched.hpp"
#include "stm32f401xe.h"
#include "thread.hpp"

// ----- Task 1 -----
void led_task_1() {
  GPIO led1(GPIOB);
  led1.set_mode(GPIOPin::Pin13, GPIOMode::Output);

  while (true) {
    led1.toggle(GPIOPin::Pin13);
    for (volatile int i = 0; i < 100000; ++i)
      ;
    Scheduler::yield();
  }
}

// ----- Task 2 -----
void led_task_2() {
  GPIO led2(GPIOB);
  led2.set_mode(GPIOPin::Pin14, GPIOMode::Output);

  while (true) {
    led2.toggle(GPIOPin::Pin14);
    for (volatile int i = 0; i < 100000; ++i)
      ;
    Scheduler::yield();
  }
}

// ----- Stacks -----
static uint32_t stack1[128];
static uint32_t stack2[128];

int main() {
  Scheduler::init();

  Thread t1(led_task_1, 1, stack1, 128);
  Thread t2(led_task_2, 1, stack2, 128);

  Scheduler::add_thread(t1.get_tcb());
  Scheduler::add_thread(t2.get_tcb());

  Scheduler::start();

  while (1) {
    __asm volatile("wfi");
  }
}
