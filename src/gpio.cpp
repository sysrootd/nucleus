#include "gpio.hpp"

GPIO::GPIO(GPIO_Type* port) : port(port) {}

void GPIO::set_mode(GPIOPin pin, GPIOMode mode) {
    int pin_number = static_cast<int>(pin);
    port->MODER &= ~(0b11 << (2 * pin_number));
    port->MODER |= (static_cast<uint32_t>(mode) << (2 * pin_number));
}

void GPIO::write(GPIOPin pin, bool value) {
    int pin_number = static_cast<int>(pin);
    if (value) {
        port->ODR |= (1 << pin_number);
    } else {
        port->ODR &= ~(1 << pin_number);
    }
}

bool GPIO::read(GPIOPin pin) {
    int pin_number = static_cast<int>(pin);
    return (port->IDR & (1 << pin_number)) != 0;
}

void GPIO::toggle(GPIOPin pin) {
    int pin_number = static_cast<int>(pin);
     port->ODR ^= (1<< pin_number);
}
