#include "adc.hpp"

void (*ADC1::user_callback)(uint16_t) = nullptr;

ADC1::ADC1(ADC_Type* adc) : adc(adc) {}

void ADC1::enable() {
    RCC->APB2ENR |= (1 << 8);  // Enable ADC1 clock
    adc->CR2 = 0;              // Disable for configuration
    adc->CR1 = 0;              // Independent mode
    adc->SQR1 = 0;             // One conversion in sequence
    adc->CR2 |= (1 << 0);      // Enable ADC
}

void ADC1::disable() {
    adc->CR2 &= ~(1 << 0);
}

void ADC1::configure_channel(uint8_t channel) {
    adc->SQR3 = channel;

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
    return (adc->SR & (1 << 1));  // EOC
}

uint16_t ADC1::read_value() const {
    return static_cast<uint16_t>(adc->DR);
}

void ADC1::enable_interrupt() {
    adc->CR1 |= (1 << 5);      // EOCIE: End of conversion interrupt enable
    NVIC_EnableIRQ(ADC_IRQn);  // Enable ADC interrupt in NVIC
}

void ADC1::disable_interrupt() {
    adc->CR1 &= ~(1 << 5);     // Disable EOC interrupt
    NVIC_DisableIRQ(ADC_IRQn);
}

void ADC1::set_callback(void (*callback)(uint16_t value)) {
    user_callback = callback;
}

// === Interrupt handler (must be in your startup vector table) ===
extern "C" void ADC_IRQHandler(void) {
    if (ADC1->SR & (1 << 1)) {  // EOC
        uint16_t value = static_cast<uint16_t>(ADC1->DR);
        if (ADC1::user_callback) {
            ADC1::user_callback(value);
        }
        ADC1->SR &= ~(1 << 1);  // Clear EOC flag
    }
}
