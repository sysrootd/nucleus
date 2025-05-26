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

    void enable_interrupt();
    void disable_interrupt();
    
    static void set_callback(void (*callback)(uint16_t value));

    static void (*user_callback)(uint16_t);


private:
    ADC_Type* adc;
};
