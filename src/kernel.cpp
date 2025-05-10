#include "uart.hpp"

void kernel_main() {
    // Initialize UART
    UART::init();

    // Send string using UART::transmit
    const char* msg = "Welcome to kernel!\r\n";
    while (*msg) {
        UART::transmit(*msg++);  // Transmit one character at a time
    }

    while (1) {
        // Main loop
    }
}

