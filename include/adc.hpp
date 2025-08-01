#pragma once
#ifndef ADC_HPP
#define ADC_HPP

#include "stm32.hpp"
#include "stm32f401xe.h"
#include <cstdint>

class Adc {
public:
  // Constructor: pass ADC1, ADC2,....
  Adc(ADC_Type *adc_instance);

  void enable();
  void disable();
  void configure_channel(uint8_t channel);
  void start_conversion();
  bool is_conversion_complete() const;
  uint16_t read_value() const;

  void enable_interrupt();
  void disable_interrupt();
  void set_callback(void (*callback)(uint16_t));

  static void (*user_callback)(uint16_t);

private:
  ADC_Type *adc;
};

#endif
