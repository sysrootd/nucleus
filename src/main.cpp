#include "stm32.h"
#include "systick.hpp"

int main() {
    // Enable GPIOC and GPIOB clocks
    RCC->AHB1ENR |= (1 << 2);  // GPIOC
    RCC->AHB1ENR |= (1 << 1);  // GPIOB

    // ================= Configure PC8 and PC9 as inputs =================
    GPIOC->MODER &= ~(0b11 << (8 * 2));  // PC8 input mode
    GPIOC->MODER &= ~(0b11 << (9 * 2));  // PC9 input mode

    // Enable pull-up resistors on PC8 and PC9
    GPIOC->PUPDR &= ~(0b11 << (8 * 2));
    GPIOC->PUPDR |=  (0b01 << (8 * 2));

    GPIOC->PUPDR &= ~(0b11 << (9 * 2));
    GPIOC->PUPDR |=  (0b01 << (9 * 2));

    // ================= Configure PB13 as output (LED) =================
    GPIOB->MODER &= ~(0b11 << (13 * 2));  // Clear mode bits
    GPIOB->MODER |=  (0b01 << (13 * 2));  // Set as output

    // ================= Initialize SysTick for delays =================
    SysTick_Init(16000000);  // Assuming system clock is 16 MHz

    while (1) {
        bool pc8_pressed = !(GPIOC->IDR & (1 << 8));  // Active low
        bool pc9_pressed = !(GPIOC->IDR & (1 << 9));

        if (pc8_pressed || pc9_pressed) {
            GPIOB->ODR ^= (1 << 13);  // Toggle PB13 LED
            delay_ms(200);            // Simple debounce delay
        }
    }
}
