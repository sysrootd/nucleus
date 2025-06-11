#include "i2c.hpp"
#include "stm32.hpp"

void I2C_Driver::init(SpeedMode mode) {
    // Enable clock for I2C peripheral
    if (i2c_ == I2C1) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    } else if (i2c_ == I2C2) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    }

    // Reset I2C
    i2c_->CR1 |= I2C_CR1_SWRST;
    i2c_->CR1 &= ~I2C_CR1_SWRST;

    // Configure timing based on speed mode
    if (mode == SpeedMode::Standard) {
        // Standard mode (100 kHz)
        i2c_->CR2 &= ~I2C_CR2_FREQ;  // Clear frequency bits
        i2c_->CR2 |= 16;              // 16 MHz peripheral clock
        i2c_->CCR = 80;               // 100 kHz
        i2c_->TRISE = 17;             // 1000 ns rise time
    } else {
        // Fast mode (400 kHz)
        i2c_->CR2 &= ~I2C_CR2_FREQ;
        i2c_->CR2 |= 16;
        i2c_->CCR = 20;               // 400 kHz
        i2c_->TRISE = 5;              // 300 ns rise time
        i2c_->CCR |= I2C_CCR_FS;      // Fast mode
    }

    // Enable I2C
    i2c_->CR1 |= I2C_CR1_PE;
}

void I2C_Driver::enable() {
    i2c_->CR1 |= I2C_CR1_PE;
}

void I2C_Driver::disable() {
    i2c_->CR1 &= ~I2C_CR1_PE;
}

void I2C_Driver::start() {
    generate_start();
    wait_for_flag(I2C_SR1_SB);
}

void I2C_Driver::stop() {
    generate_stop();
}

void I2C_Driver::write(uint8_t data) {
    i2c_->DR = data;
    wait_for_flag(I2C_SR1_TXE);
}

uint8_t I2C_Driver::read(AckControl ack) {
    if (ack == AckControl::Enable) {
        i2c_->CR1 |= I2C_CR1_ACK;
    } else {
        i2c_->CR1 &= ~I2C_CR1_ACK;
    }
    
    wait_for_flag(I2C_SR1_RXNE);
    return i2c_->DR;
}

bool I2C_Driver::is_busy() {
    return (i2c_->SR2 & I2C_SR2_BUSY);
}

bool I2C_Driver::check_event(uint32_t event) {
    uint32_t lastevent = 0;
    uint32_t flag1 = 0, flag2 = 0;
    
    flag1 = i2c_->SR1;
    flag2 = i2c_->SR2;
    flag2 = flag2 << 16;
    
    lastevent = (flag1 | flag2) & FLAG_MASK;
    
    if (lastevent == event) {
        return true;
    } else {
        return false;
    }
}

// Private methods
void I2C_Driver::generate_start() {
    i2c_->CR1 |= I2C_CR1_START;
}

void I2C_Driver::generate_stop() {
    i2c_->CR1 |= I2C_CR1_STOP;
}

void I2C_Driver::wait_for_flag(uint32_t flag) {
    uint32_t timeout = 100000;
    while (!(i2c_->SR1 & flag)) {
        if (--timeout == 0) {
            // Handle timeout error
            break;
        }
    }
}


bool I2C_Driver::write_register(uint8_t reg_addr, uint8_t data) {
    generate_start();
    write(device_address_ & ~0x01); // Write operation
    write(reg_addr);
    write(data);
    generate_stop();
    return true;
}

bool I2C_Driver::read_register(uint8_t reg_addr, uint8_t& data) {
    generate_start();
    write(device_address_ & ~0x01); // Write operation
    write(reg_addr);
    
    generate_start(); // Repeated start
    write(device_address_ | 0x01); // Read operation
    data = read(AckControl::Disable);
    generate_stop();
    return true;
}

bool I2C_Driver::write_data(uint8_t reg_addr, const uint8_t* data, size_t len) {
    generate_start();
    write(device_address_ & ~0x01);
    write(reg_addr);
    
    for(size_t i = 0; i < len; i++) {
        write(data[i]);
    }
    
    generate_stop();
    return true;
}

bool I2C_Driver::read_data(uint8_t reg_addr, uint8_t* buffer, size_t len) {
    generate_start();
    write(device_address_ & ~0x01);
    write(reg_addr);
    
    generate_start(); // Repeated start
    write(device_address_ | 0x01);
    
    for(size_t i = 0; i < len; i++) {
        buffer[i] = read((i == len-1) ? AckControl::Disable : AckControl::Enable);
    }
    
    generate_stop();
    return true;
}

bool I2C_Driver::probe_device() {
    generate_start();
    bool ack = check_event(I2C_SR1_ADDR);
    generate_stop();
    return ack;
}
