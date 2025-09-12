#include "gpio.hpp"
#include "sched.hpp"
#include "stm32f401xe.h"
#include "thread.hpp"


void Red_led() {
  GPIO led1 = GPIO(GPIOB);
  led1.set_mode(GPIOPin::Pin13, GPIOMode::Output);

  while (true) {
    led1.toggle(GPIOPin::Pin13);
    for (volatile int i = 0; i < 100000; ++i)
      ;
    Scheduler::yield();
  }
}

void Green_led() {
  GPIO led2 = GPIO(GPIOB);
  led2.set_mode(GPIOPin::Pin14, GPIOMode::Output);

  while (true) {
    led2.toggle(GPIOPin::Pin14);
    for (volatile int i = 0; i < 100000; ++i)
      ;
    Scheduler::yield();
  }
}

static uint32_t stack1[128];
static uint32_t stack2[128];

int main() {
  Scheduler::init();

  Thread t1(Red_led, 1, stack1, 128);
  Thread t2(Green_led, 1, stack2, 128);

  Scheduler::add_thread(t1.get_tcb());
  Scheduler::add_thread(t2.get_tcb());

  Scheduler::start();

  while (1) {
    __asm volatile("wfi");
  }
}
