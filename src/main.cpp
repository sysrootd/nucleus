#include "uart.hpp"

int main() {
    // Initialize UART
    UART::init();

    // Print a test message
    UART::uart_printf("Hello, STM32F401RBT6!\n");

    while (1) {
        // Wait for data or implement other logic
    }

    return 0;
}

