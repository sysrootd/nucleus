#pragma once

#include <cstdint>

void SysTick_Init(uint32_t system_clock_hz = 16000000);
void delay_ms(uint32_t ms);
uint32_t systick_now(); 
