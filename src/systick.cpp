#include "systick.hpp"
#include "stm32.h"
#include "stm32f401xe.h"
#include "core_cm4.h"  //__WFI()


volatile uint32_t tick_ms = 0;

void SysTick_Init(uint32_t system_clock_hz) {
    STK->LOAD = (system_clock_hz / 1000) - 1; // 1ms tick
    STK->VAL = 0;
    STK->CTRL = 0x07; // Enable SysTick with interrupt
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

