#include "stm32.hpp"
#include "uart.hpp"
#include <cstdarg>

void UART2::init() {
    RCC->AHB1ENR |= (1 << 0); // GPIOAEN

    // 2. Set PA2 and PA3 to alternate function mode
    GPIOA->MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2))); // clear MODER2 and MODER3
    GPIOA->MODER |=  ((2 << (2 * 2)) | (2 << (3 * 2))); // set to AF mode (10)

    // 3. Set PA2 and PA3 to AF7 (USART2)
    GPIOA->AFRL &= ~((0xF << (4 * 2)) | (0xF << (4 * 3))); // clear AFRL2, AFRL3
    GPIOA->AFRL |=  ((7 << (4 * 2)) | (7 << (4 * 3)));     // set AF7

    // 4. Enable USART2 clock
    RCC->APB1ENR |= (1 << 17); // USART2EN

    // Configure baud rate to 9600 (assuming 16 MHz APB2 clock)
    USART2->BRR = 0x8b;  //115200 //0x683;  // BRR = 16 MHz / 9600

    // Enable the UART, transmitter, and receiver
    USART2->CR1 = (1 << 13) | (1 << 3) | (1 << 2);
}

void UART2::transmit(uint8_t data) {
    // Wait for the TXE bit to be set (TX register empty)
    while (!(USART2->SR & (1 << 7)));

    // Write data to the data register
    USART2->DR = data;
}

uint8_t UART2::receive() {
    // Wait for the RXNE bit to be set (RX register not empty)
    while (!(USART2->SR & (1 << 5)));

    // Read data from the data register
    return static_cast<uint8_t>(USART2->DR);
}



void UART2::uart_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'c') {
                char c = static_cast<char>(va_arg(args, int));
                UART2::transmit(c);
            }
            else if (*fmt == 's') {
                const char* str = va_arg(args, const char*);
                while (*str) UART2::transmit(*str++);
            }
            else if (*fmt == 'd') {
                int val = va_arg(args, int);
                char buf[12]; // enough for 32-bit signed int
                int i = 0;
                bool neg = false;

                if (val < 0) {
                    neg = true;
                    val = -val;
                }

                do {
                    buf[i++] = (val % 10) + '0';
                    val /= 10;
                } while (val);

                if (neg) buf[i++] = '-';

                while (i--) UART2::transmit(buf[i]);
            }
        } else {
            if (*fmt == '\n') {
                UART2::transmit('\r'); // carriage return before newline
            }
            UART2::transmit(*fmt);
        }
        fmt++;
    }

    va_end(args);
}


