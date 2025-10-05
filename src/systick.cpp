#include "systick.hpp"
#include "stm32f401xe.h"
#include "rtos.hpp"

static volatile uint32_t tick_ms = 0;

void SysTick_Init(uint32_t system_clock_hz) {
    SysTick->LOAD = (system_clock_hz / 1000) - 1; // 1 ms tick
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | // CPU clock
                    SysTick_CTRL_TICKINT_Msk   | // Enable interrupt
                    SysTick_CTRL_ENABLE_Msk;     // Enable counter
}

extern "C" void SysTick_Handler(void) {
    ++tick_ms;
    rtos_tick_handler();   // notify RTOS on every tick
}

uint32_t systick_now() {
    return tick_ms;
}

void delay_ms(uint32_t ms) {
    uint32_t start = tick_ms;
    while ((tick_ms - start) < ms) {
        __WFI();  // Wait for interrupt (saves power)
    }
}
