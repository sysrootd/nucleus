#include "gpio.hpp"
#include "timer.hpp"
#include "stm32.hpp"

int main() {
    // Enable GPIOB clock
    RCC->AHB1ENR |= (1 << 1);

    // Create GPIO instance for GPIOB port
    GPIO gpioB = GPIO(GPIOB);

    // Configure PB13 and PB14 as output
    gpioB.set_mode(GPIOPin::Pin13, GPIOMode::Output);
    gpioB.set_mode(GPIOPin::Pin14, GPIOMode::Output);

    // Use Timer2 from timer.hpp directly
    using Timer2 = Timer<TIM2_Info>;

    // Initialize Timer2 with prescaler and auto reload for 1 second overflow
    Timer2::init(16000 - 1, 1000 - 1);

    while (1) {
        // Check if update interrupt flag (UIF) is set, means timer overflowed
        if (TIM2->SR & (1 << 0)) {
            TIM2->SR &= ~(1 << 0);  // Clear the flag

            static bool led_state = false;
            led_state = !led_state;

            gpioB.write(GPIOPin::Pin13, led_state);
            gpioB.write(GPIOPin::Pin14, !led_state);
        }
    }

    return 0;
}
