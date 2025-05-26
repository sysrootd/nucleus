#define __NVIC_PRIO_BITS 4  // Required before core_cm4.h
#include "stm32.hpp"
#include "stm32f401xe.h"  // Provides IRQn_Type, ADC_IRQn, and all peripheral base addresses
#include "core_cm4.h"     // Provides NVIC functions
#include "adc.hpp"  // Your ADC class


void (*ADC1::user_callback)(uint16_t) = nullptr;

ADC1::ADC1(ADC_Type* adc) : adc(adc) {}

void ADC1::enable() {
    RCC->APB2ENR |= (1 << 8);  // Enable ADC1 clock
    adc->CR2 = 0;              // Disable for configuration
    adc->CR1 = 0;              // Independent mode
    adc->SQR1 = 0;             // One conversion
    adc->CR2 |= (1 << 0);      // Enable ADC
}

void ADC1::disable() {
    adc->CR2 &= ~(1 << 0);
}

void ADC1::configure_channel(uint8_t channel) {
    adc->SQR3 = channel;
    if (channel <= 9) {
        adc->SMPR2 &= ~(0b111 << (channel * 3));
        adc->SMPR2 |=  (0b100 << (channel * 3));
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
    adc->CR1 |= (1 << 5);      // EOCIE
    NVIC_EnableIRQ(ADC_IRQn);  // Enable interrupt
}

void ADC1::disable_interrupt() {
    adc->CR1 &= ~(1 << 5);
    NVIC_DisableIRQ(ADC_IRQn);
}

void ADC1::set_callback(void (*callback)(uint16_t)) {
    user_callback = callback;
}

// === Correct interrupt handler ===
extern "C" void ADC_IRQHandler(void) {
    if (ADC->SR & (1 << 1)) {  // EOC
        uint16_t value = static_cast<uint16_t>(ADC->DR);
        if (ADC1::user_callback) {
            ADC1::user_callback(value);
        }
        ADC->SR &= ~(1 << 1);  // Clear EOC flag
    }
}
