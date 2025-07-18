#include "gpio.hpp"

GPIO::GPIO(GPIO_Type *port) : port(port) {
  // Enable GPIO clock based on port
  if (port == GPIOA)
    RCC->AHB1ENR |= (1 << 0);
  else if (port == GPIOB)
    RCC->AHB1ENR |= (1 << 1);
  else if (port == GPIOC)
    RCC->AHB1ENR |= (1 << 2);
}

void GPIO::set_mode(GPIOPin pin, GPIOMode mode) {
  uint8_t pin_num = static_cast<uint8_t>(pin);
  uint8_t mode_bits = static_cast<uint8_t>(mode);
  port->MODER &= ~(0b11 << (pin_num * 2));
  port->MODER |= (mode_bits << (pin_num * 2));
}

void GPIO::write(GPIOPin pin, bool value) {
  uint8_t pin_num = static_cast<uint8_t>(pin);
  if (value)
    port->ODR |= (1 << pin_num);
  else
    port->ODR &= ~(1 << pin_num);
}

bool GPIO::read(GPIOPin pin) {
  uint8_t pin_num = static_cast<uint8_t>(pin);
  return (port->IDR & (1 << pin_num));
}

void GPIO::toggle(GPIOPin pin) {
  uint8_t pin_num = static_cast<uint8_t>(pin);
  port->ODR ^= (1 << pin_num);
}
