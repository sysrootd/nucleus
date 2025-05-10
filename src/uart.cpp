#include "uart.hpp"
#include <cstdarg>

// UART1 Base Address (STM32F401)
#define UART1_BASE 0x40011000U
#define UART_SR (volatile uint32_t*)(UART1_BASE + 0x00)
#define UART_DR (volatile uint32_t*)(UART1_BASE + 0x04)
#define UART_BRR (volatile uint32_t*)(UART1_BASE + 0x08)
#define UART_CR1 (volatile uint32_t*)(UART1_BASE + 0x0C)

// UART register bit definitions
#define UART_CR1_UE (1 << 13)  // USART enable
#define UART_CR1_TE (1 << 3)   // Transmitter enable
#define UART_CR1_RE (1 << 2)   // Receiver enable
#define UART_SR_TXE (1 << 7)   // Transmit data register empty
#define UART_SR_RXNE (1 << 5)  // Read data register not empty

void UART::init() {
    // Enable UART1 clock (this can be done in RCC)
    // Simplified; you'll need to configure the clock in actual code

    // Configure baud rate to 9600 (assuming 16 MHz APB2 clock)
    *UART_BRR = 0x683;  // BRR = 16 MHz / 9600

    // Enable the UART, transmitter, and receiver
    *UART_CR1 = UART_CR1_UE | UART_CR1_TE | UART_CR1_RE;
}

void UART::transmit(uint8_t data) {
    // Wait for the TXE bit to be set (TX register empty)
    while (!(*UART_SR & UART_SR_TXE));

    // Write data to the data register
    *UART_DR = data;
}

uint8_t UART::receive() {
    // Wait for the RXNE bit to be set (RX register not empty)
    while (!(*UART_SR & UART_SR_RXNE));

    // Read data from the data register
    return static_cast<uint8_t>(*UART_DR);
}


// Custom printf for bare-metal STM32
void UART::uart_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'c') {
                // Print single character
                char c = static_cast<char>(va_arg(args, int));
                UART::transmit(c);
            }
            else if (*fmt == 's') {
                // Print string
                const char* str = va_arg(args, const char*);
                while (*str) {
                    UART::transmit(*str++);
                }
            }
        }
        else {
            // Print regular character
            UART::transmit(*fmt);
        }
        fmt++;
    }

    va_end(args);
}

