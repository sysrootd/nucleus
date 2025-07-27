#include "uart.hpp"

void kernel_main() {
  // Initialize UART
  UART2::init();

  // Send string using UART2::transmit
  const char *msg = "Welcome to kernel!\r\n";
  while (*msg) {
    UART2::transmit(*msg++); // Transmit one character at a time
  }

  while (true)
    ;
}
