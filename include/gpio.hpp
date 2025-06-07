#pragma once

#include "stm32.hpp"

enum class GPIOPin {
    Pin0 = 0,
    Pin1, Pin2, Pin3, Pin4,
    Pin5, Pin6, Pin7, Pin8,
    Pin9, Pin10, Pin11, Pin12,
    Pin13, Pin14, Pin15
};

enum class GPIOMode {
    Input = 0b00,
    Output = 0b01,
    Alternate = 0b10,
    Analog = 0b11
};

class GPIO {
public:
    GPIO(GPIO_Type* port);
    void set_mode(GPIOPin pin, GPIOMode mode);
    void write(GPIOPin pin, bool value);
    bool read(GPIOPin pin);
    void toggle(GPIOPin pin);

private:
    GPIO_Type* port;
};
