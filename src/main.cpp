// #include "stm32.h"
// #include "uart.hpp"
// #include "gpio.hpp"

// int main() {
//     // Enable GPIOB clock
//     RCC->AHB1ENR |= (1 << 1);

//     // Use CMSIS-style GPIOA pointer
//     GPIO GPIOB(GPIOB);
//     gpioB.set_mode(GPIOPin::Pin13, GPIOMode::Output);

//     while (1) {
//         gpioB.write(GPIOPin::Pin13, true);
//         for (volatile int i = 0; i < 100000; ++i);
//         gpioB.write(GPIOPin::Pin13, false);
//         for (volatile int i = 0; i < 100000; ++i);
//     }
// }


#include "stm32.h"
#include "uart.hpp"
#include "gpio.hpp"

int main() {
    // Enable GPIOB clock
    RCC->AHB1ENR |= (1 << 1);  // Bit 1 enables GPIOB

    GPIO gpioB(GPIOB);  // Proper object instantiation
    gpioB.set_mode(GPIOPin::Pin13, GPIOMode::Output);

    while (1) {
        gpioB.write(GPIOPin::Pin13, true);
        for (volatile int i = 0; i < 100000; ++i);
        gpioB.write(GPIOPin::Pin13, false);
        for (volatile int i = 0; i < 100000; ++i);
    }
}
