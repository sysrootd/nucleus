#include "uart.hpp"
#include <stdint.h>

#define USART2_BASE 0x40004400
#define RCC_BASE    0x40023800
#define GPIOA_BASE  0x40020000

#define USART2_SR   *(volatile uint32_t*)(USART2_BASE + 0x00)
#define USART2_DR   *(volatile uint32_t*)(USART2_BASE + 0x04)
#define USART2_BRR  *(volatile uint32_t*)(USART2_BASE + 0x08)
#define USART2_CR1  *(volatile uint32_t*)(USART2_BASE + 0x0C)

#define RCC_AHB1ENR *(volatile uint32_t*)(RCC_BASE + 0x30)
#define RCC_APB1ENR *(volatile uint32_t*)(RCC_BASE + 0x40)

#define GPIOA_MODER *(volatile uint32_t*)(GPIOA_BASE + 0x00)
#define GPIOA_AFRL  *(volatile uint32_t*)(GPIOA_BASE + 0x20)

void uart_init() {
    RCC_AHB1ENR |= (1 << 0);    // Enable GPIOA
    RCC_APB1ENR |= (1 << 17);   // Enable USART2

    GPIOA_MODER &= ~(0b11 << (2 * 2));
    GPIOA_MODER |=  (0b10 << (2 * 2));   // PA2 to alternate function

    GPIOA_AFRL &= ~(0xF << (4 * 2));
    GPIOA_AFRL |=  (0x7 << (4 * 2));     // AF7 (USART2) on PA2

    USART2_BRR = 0x0683; // 9600 baud at 16 MHz
    USART2_CR1 = (1 << 13) | (1 << 3) | (1 << 2); // UE, TE, RE
}

void uart_write(const char* str) {
    while (*str) {
        while (!(USART2_SR & (1 << 7))); // Wait for TXE
        USART2_DR = *str++;
    }
}

