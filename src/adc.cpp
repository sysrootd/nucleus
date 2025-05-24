#include "adc.hpp"

ADC1::ADC1(ADC_Type* adc) : adc(adc) {}

void ADC1::enable() {
    // Enable ADC1 clock (bit 8 on APB2ENR)
    RCC->APB2ENR |= (1 << 8);

    // ADC configuration
    adc->CR2 = 0;  // Disable to configure
    adc->CR1 = 0;  // Independent mode, 12-bit resolution
    adc->SQR1 = 0; // Only one conversion
    adc->CR2 |= (1 << 0);  // ADON: Enable ADC
}

void ADC1::disable() {
    adc->CR2 &= ~(1 << 0);  // ADON: Disable ADC
}

void ADC1::configure_channel(uint8_t channel) {
    // Select channel in regular sequence
    adc->SQR3 = channel;

    // Set sampling time for selected channel (example: 84 cycles)
    if (channel <= 9) {
        adc->SMPR2 &= ~(0b111 << (channel * 3));
        adc->SMPR2 |=  (0b100 << (channel * 3));  // 84 cycles
    } else {
        uint8_t ch = channel - 10;
        adc->SMPR1 &= ~(0b111 << (ch * 3));
        adc->SMPR1 |=  (0b100 << (ch * 3));
    }
}

void ADC1::start_conversion() {
    adc->CR2 |= (1 << 30);  // SWSTART
}

bool ADC1::is_conversion_complete() const {
    return (adc->SR & (1 << 1));  // EOC: End of conversion
}

uint16_t ADC1::read_value() const {
    return static_cast<uint16_t>(adc->DR);
}
