#pragma once
#include "stm32.hpp"
#include <cstdint>

class SPI_Driver {
public:
    enum class Mode {
        Mode0,  // CPOL=0, CPHA=0
        Mode1,  // CPOL=0, CPHA=1
        Mode2,  // CPOL=1, CPHA=0
        Mode3   // CPOL=1, CPHA=1
    };

    enum class DataSize {
        Bits8,
        Bits16
    };

    enum class BaudRate {
        Div2,
        Div4,
        Div8,
        Div16,
        Div32,
        Div64,
        Div128,
        Div256
    };

    SPI_Driver(SPI_Type* spi) : spi_(spi) {}

    void init(Mode mode = Mode::Mode0, 
              DataSize dataSize = DataSize::Bits8,
              BaudRate baud = BaudRate::Div2);
    void enable();
    void disable();
    uint8_t transmit_receive(uint8_t data);
    uint16_t transmit_receive(uint16_t data);
    void set_data_size(DataSize size);
    void set_baud_rate(BaudRate rate);

private:
    SPI_Type* spi_;
};