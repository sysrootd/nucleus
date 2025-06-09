#define __NVIC_PRIO_BITS 4  // Must be defined before including core_cm4.h
#include "adc.hpp"
#include "stm32f401xe.h"     // For ADC1, RCC, ADC_IRQn
#include "core_cm4.h"        // For NVIC functions


// Static callback pointer initialization
void (*Adc::user_callback)(uint16_t) = nullptr;

Adc::Adc(ADC_Type* adc) : adc(adc) {}

// Enable ADC and configure basic settings
void Adc::enable() {
    RCC->APB2ENR |= (1 << 8);  // Enable ADC1 clock
    adc->CR2 = 0;              // Reset CR2
    adc->CR1 = 0;              // Reset CR1
    adc->SQR1 = 0;             // One conversion in regular sequence
    adc->CR2 |= (1 << 0);      // Enable ADC
}

// Disable ADC
void Adc::disable() {
    adc->CR2 &= ~(1 << 0);     // Clear ADON bit
}

// Configure channel and sample time
void Adc::configure_channel(uint8_t channel) {
    adc->SQR3 = channel;  // Select channel for 1st conversion
    if (channel <= 9) {
        adc->SMPR2 &= ~(0b111 << (channel * 3));
        adc->SMPR2 |=  (0b100 << (channel * 3));  // 84 ADC cycles
    } else {
        uint8_t ch = channel - 10;
        adc->SMPR1 &= ~(0b111 << (ch * 3));
        adc->SMPR1 |=  (0b100 << (ch * 3));       // 84 ADC cycles
    }
}

// Start single conversion
void Adc::start_conversion() {
    adc->CR2 |= (1 << 30);  // Set SWSTART
}

// Check if conversion is complete
bool Adc::is_conversion_complete() const {
    return (adc->SR & (1 << 1));  // EOC flag
}

// Read ADC conversion result
uint16_t Adc::read_value() const {
    return static_cast<uint16_t>(adc->DR);
}

// Enable end-of-conversion interrupt
void Adc::enable_interrupt() {
    adc->CR1 |= (1 << 5);        // EOCIE
    NVIC_EnableIRQ(ADC_IRQn);   // Enable IRQ in NVIC
}

// Disable end-of-conversion interrupt
void Adc::disable_interrupt() {
    adc->CR1 &= ~(1 << 5);       // Clear EOCIE
    NVIC_DisableIRQ(ADC_IRQn);  // Disable NVIC interrupt
}

// Register a user callback
void Adc::set_callback(void (*callback)(uint16_t value)) {
    user_callback = callback;
}

// === ADC Interrupt Handler ===
extern "C" void ADC_IRQHandler(void) {
    if (ADC1->SR & (1 << 1)) {  // Check EOC
        uint16_t value = static_cast<uint16_t>(ADC1->DR);
        if (Adc::user_callback) {
            Adc::user_callback(value);
        }
        ADC1->SR &= ~(1 << 1);  // Clear EOC flag
    }
}
