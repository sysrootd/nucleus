#pragma once
#include "stm32.hpp"

class ADC1 {
public:
    explicit ADC1(ADC_Type* adc);

    void enable();
    void disable();
    void configure_channel(uint8_t channel);
    void start_conversion();
    bool is_conversion_complete() const;
    uint16_t read_value() const;

private:
    ADC_Type* adc;
};
