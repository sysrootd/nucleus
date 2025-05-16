#include "stm32.h"
#include "uart.hpp"

int main() {

    UART2::init();
    UART2::uart_printf("Hello I'm Rakesh: %d\n", 007);
    while (1) {
        char c = UART2::receive();
        UART2::transmit(c); // echo
    }
}
