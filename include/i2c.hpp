#pragma once
#include "stm32.hpp"
#include <cstdint>
#include <array>



class I2C_Driver{
public:
    enum class SpeedMode {
        Standard,   // 100 kHz
        Fast        // 400 kHz
    };

    I2C_Driver(I2C_Type* i2c, uint8_t dev_addr) 
        : i2c_(i2c), device_address_(dev_addr << 1) {}

    // Initialization
    void init(SpeedMode mode = SpeedMode::Standard);
    
    // Basic operations
    bool write_register(uint8_t reg_addr, uint8_t data);
    bool read_register(uint8_t reg_addr, uint8_t& data);
    bool write_data(uint8_t reg_addr, const uint8_t* data, size_t len);
    bool read_data(uint8_t reg_addr, uint8_t* buffer, size_t len);

    // Advanced operations
    bool probe_device();
    bool write_only(const uint8_t* data, size_t len);
    bool read_only(uint8_t* buffer, size_t len);

private:
    I2C_Type* i2c_;
    uint8_t device_address_;
    
    void generate_start();
    void generate_stop();
    void wait_for_flag(uint32_t flag);
    bool check_event(uint32_t event);
};