#include "systick.hpp"
#include "sched.hpp"
#include "stm32.hpp"
#include "stm32f401xe.h"

volatile uint32_t tick_ms = 0;

void SysTick_Init(uint32_t system_clock_hz) {
  SysTick->LOAD = (system_clock_hz / 1000) - 1; // 1ms tick
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk // Processor clock
                  | SysTick_CTRL_TICKINT_Msk // Enable interrupt
                  | SysTick_CTRL_ENABLE_Msk; // Enable SysTick
}

extern "C" void SysTick_Handler(void) {
  ++tick_ms;
  Scheduler::tick(); // Update sleep timers and wake sleeping threads

  // Trigger context switch after tick update
  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void delay_ms(uint32_t ms) {
  uint32_t start = tick_ms;
  while ((tick_ms - start) < ms) {
    __WFI();
  }
}
