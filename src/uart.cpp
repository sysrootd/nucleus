#include "stm32.h"
#include "uart.hpp"
#include <cstdarg>




void UART2::init() {
    // Enable UART1 clock (this can be done in RCC)
    // Simplified; you'll need to configure the clock in actual code

    // Configure baud rate to 9600 (assuming 16 MHz APB2 clock)
    USART2->BRR = 0x683;  // BRR = 16 MHz / 9600

    // Enable the UART, transmitter, and receiver
    USART2->CR1 = 0x01;
}

void UART2::transmit(uint8_t data) {
    // Wait for the TXE bit to be set (TX register empty)
    while (!(USART2->SR & 0x01));

    // Write data to the data register
    USART2->DR = data;
}

uint8_t UART2::receive() {
    // Wait for the RXNE bit to be set (RX register not empty)
    while (!(USART2->SR & 0x01));

    // Read data from the data register
    return static_cast<uint8_t>(USART2->DR);
}


// Custom printf for bare-metal STM32
void UART2::uart_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'c') {
                // Print single character
                char c = static_cast<char>(va_arg(args, int));
                UART2::transmit(c);
            }
            else if (*fmt == 's') {
                // Print string
                const char* str = va_arg(args, const char*);
                while (*str) {
                    UART2::transmit(*str++);
                }
            }
        }
        else {
            // Print regular character
            UART2::transmit(*fmt);
        }
        fmt++;
    }

    va_end(args);
}

