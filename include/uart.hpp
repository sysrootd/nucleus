#ifndef UART_HPP
#define UART_HPP

#include <stdint.h>

class UART2 {
public:
  static void init();
  static void transmit(uint8_t data);
  static uint8_t receive();
  static void uart_printf(const char *fmt, ...);
};

#endif // UART_HPP