#pragma once

#include <cstdint>

enum class EXTI_Trigger { Rising, Falling, Both };

class EXTI {
public:
  static void enable(uint8_t line, EXTI_Trigger trigger);
  static void enable_interrupt(uint8_t line);
  static void disable_interrupt(uint8_t line);
};
