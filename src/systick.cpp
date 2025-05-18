#include "systick.hpp"
#include "stm32.hpp"
#include "stm32f401xe.h"
#include "core_cm4.h"  //__WFI()

volatile uint32_t tick_ms = 0;

void SysTick_Init(uint32_t system_clock_hz) {
    SysTick->LOAD = (system_clock_hz / 1000) - 1;  // 1ms tick
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk  // processor clock
                  | SysTick_CTRL_TICKINT_Msk    // enable interrupt
                  | SysTick_CTRL_ENABLE_Msk;    // enable SysTick
}

extern "C" void SysTick_Handler(void) {
    ++tick_ms;
}

void delay_ms(uint32_t ms) {
    uint32_t start = tick_ms;
    while ((tick_ms - start) < ms) {
        __WFI();  // Sleep until SysTick interrupt
    }
}

