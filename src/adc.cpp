#define __NVIC_PRIO_BITS 4      // Required before core_cm4.h
#include "stm32.hpp"
#include "stm32f401xe.h"        // Provides IRQn_Type, ADC_IRQn, and all peripheral base addresses
#include "core_cm4.h"           // Provides NVIC functions
#include "adc.hpp"              // Your ADC class
#include "stm32f401xe.h"        // For ADC1 and ADC_IRQn
#include "core_cm4.h"           // For NVIC_EnableIRQ and NVIC_DisableIRQ
#include "adc.hpp"

void (*Adc::user_callback)(uint16_t) = nullptr;

Adc::Adc(ADC_Type* adc) : adc(adc) {}

void Adc::enable() {
    RCC->APB2ENR |= (1 << 8);  // Enable ADC1 clock
    adc->CR2 = 0;              // Disable for configuration
    adc->CR1 = 0;              // Independent mode
    adc->SQR1 = 0;             // One conversion
    adc->CR2 |= (1 << 0);      // Enable ADC
}

void Adc::disable() {
    adc->CR2 &= ~(1 << 0);
}

void Adc::configure_channel(uint8_t channel) {
    adc->SQR3 = channel;
    if (channel <= 9) {
        adc->SMPR2 &= ~(0b111 << (channel * 3));
        adc->SMPR2 |=  (0b100 << (channel * 3));
        adc->SMPR2 |=  (0b100 << (channel * 3));  // 84 ADC cycles
    } else {
        uint8_t ch = channel - 10;
        adc->SMPR1 &= ~(0b111 << (ch * 3));
        adc->SMPR1 |=  (0b100 << (ch * 3));
    }
}

void Adc::start_conversion() {
    adc->CR2 |= (1 << 30);  // SWSTART
}

bool Adc::is_conversion_complete() const {
    return (adc->SR & (1 << 1));  // EOC flag
}

uint16_t Adc::read_value() const {
    return static_cast<uint16_t>(adc->DR);
}

void Adc::enable_interrupt() {
    adc->CR1 |= (1 << 5);        // EOCIE: End of conversion interrupt enable
    NVIC_EnableIRQ(ADC_IRQn);   // Enable ADC interrupt in NVIC
}

void Adc::disable_interrupt() {
    adc->CR1 &= ~(1 << 5);       // Disable EOC interrupt
    NVIC_DisableIRQ(ADC_IRQn);  // Disable NVIC interrupt
}

void Adc::set_callback(void (*callback)(uint16_t value)) {
    user_callback = callback;
}

// === interrupt handler ===
// === ADC Interrupt Handler ===
extern "C" void ADC_IRQHandler(void) {
    if (ADC->SR & (1 << 1)) {  // Check EOC flag
        uint16_t value = static_cast<uint16_t>(ADC->DR);
        if (Adc::user_callback) {
            Adc::user_callback(value);
        }
        ADC->SR &= ~(1 << 1);  // Clear EOC flag
    }
}
