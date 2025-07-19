#include "gpio.hpp"
#include "sched.hpp"
#include "stm32.hpp"
#include "systick.hpp"
#include "thread.hpp"
#include <cstddef>

#include "gpio.hpp"
#include "sched.hpp"
#include "stm32.hpp"
#include "systick.hpp"
#include "thread.hpp"
#include <cstddef>

GPIO *gpioB = nullptr;

// Forward declarations
void led_task_1();
void led_task_2();

constexpr size_t STACK_WORDS = 512;
alignas(8) static uint32_t stack1[STACK_WORDS];
alignas(8) static uint32_t stack2[STACK_WORDS];

int main() {
  static GPIO gpio(GPIOB);
  gpioB = &gpio;

  SysTick_Init();
  Scheduler::init();

  // 🔍 Debug: Check entry addresses
  volatile uint32_t t1_addr = reinterpret_cast<uint32_t>(led_task_1);
  volatile uint32_t t2_addr = reinterpret_cast<uint32_t>(led_task_2);

  if (t1_addr < 0x08000000 || t1_addr > 0x08080000) {
    *((volatile uint32_t *)0x40020400) |= (1 << 28); // signal t1 invalid
    while (true)
      ;
  }

  if (t2_addr < 0x08000000 || t2_addr > 0x08080000) {
    *((volatile uint32_t *)0x40020400) |= (1 << 26); // signal t2 invalid
    while (true)
      ;
  }

  static Thread t1(led_task_1, 1, stack1, STACK_WORDS);
  static Thread t2(led_task_2, 1, stack2, STACK_WORDS);

  Scheduler::add_thread(t1.get_tcb());
  Scheduler::add_thread(t2.get_tcb());

  Scheduler::start();

  while (true)
    ;
}

void led_task_1() {
  while (true) {
    if (gpioB) {
      gpioB->toggle(GPIOPin::Pin13);
      Scheduler::sleep(500);
    }
  }
}

void led_task_2() {
  while (true) {
    if (gpioB) {
      gpioB->toggle(GPIOPin::Pin14);
      Scheduler::sleep(1000);
    }
  }
}
