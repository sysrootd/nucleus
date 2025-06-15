#include "stm32.hpp"
#include "i2c.hpp"

I2C_Driver::I2C_Driver(I2C_Type* i2c, uint8_t device_address)
    : i2c_(i2c), device_address(device_address) {}

void I2C_Driver::init(SpeedMode mode) {
    if (i2c_ == I2C1) {
        *(volatile uint32_t*)0x40023840 |= (1 << 21);  // Enable I2C1 clock
    } else if (i2c_ == I2C2) {
        *(volatile uint32_t*)0x40023840 |= (1 << 22);  // Enable I2C2 clock
    }

    i2c_->CR1 |= (1 << 15);
    i2c_->CR1 &= ~(1 << 15);

    i2c_->CR2 &= ~0x3F;
    i2c_->CR2 |= 16;

    if (mode == SpeedMode::Standard) {
        i2c_->CCR = 80;
        i2c_->TRISE = 17;
    } else {
        i2c_->CCR = 20;
        i2c_->CCR |= (1 << 15);
        i2c_->TRISE = 5;
    }

    i2c_->CR1 |= (1 << 0);
} 

void I2C_Driver::enable() {
    i2c_->CR1 |= (0x01 << 0);  // PE
}

void I2C_Driver::disable() {
    i2c_->CR1 &= ~(0x01 << 0);  // Clear PE
}

void I2C_Driver::start() {
    generate_start();
    wait_for_flag(0x0001);  // SB (bit 0 in SR1)
}

void I2C_Driver::stop() {
    generate_stop();
}

void I2C_Driver::write(uint8_t data) {
    i2c_->DR = data;
    wait_for_flag(0x0080);  // TXE (bit 7 in SR1)
}

uint8_t I2C_Driver::read(AckControl ack) {
    if (ack == AckControl::Enable) {
        i2c_->CR1 |= (0x01 << 10);  // ACK
    } else {
        i2c_->CR1 &= ~(0x01 << 10);
    }

    wait_for_flag(0x0040);  // RXNE (bit 6 in SR1)
    return i2c_->DR;
}

bool I2C_Driver::is_busy() {
    return (i2c_->SR2 & (0x01 << 1));  // BUSY (bit 1)
}

bool I2C_Driver::check_event(uint32_t event) {
    uint32_t flag1 = i2c_->SR1;
    uint32_t flag2 = i2c_->SR2 << 16;
    uint32_t lastevent = (flag1 | flag2) & FLAG_MASK;
    return (lastevent == event);
}

// --- Private ---

void I2C_Driver::generate_start() {
    i2c_->CR1 |= (0x01 << 8);  // START
}

void I2C_Driver::generate_stop() {
    i2c_->CR1 |= (0x01 << 9);  // STOP
}

void I2C_Driver::wait_for_flag(uint32_t flag) {
    uint32_t timeout = 100000;
    while (!(i2c_->SR1 & flag)) {
        if (--timeout == 0) break;  // Timeout
    }
}

bool I2C_Driver::write_register(uint8_t reg_addr, uint8_t data) {
    generate_start();
    write(device_address & ~0x01);  // Write mode
    write(reg_addr);
    write(data);
    generate_stop();
    return true;
}

bool I2C_Driver::read_register(uint8_t reg_addr, uint8_t& data) {
    generate_start();
    write(device_address & ~0x01);  // Write
    write(reg_addr);

    generate_start();                // Repeated START
    write(device_address | 0x01);   // Read
    data = read(AckControl::Disable);
    generate_stop();
    return true;
}

bool I2C_Driver::write_data(uint8_t reg_addr, const uint8_t* data, size_t len) {
    generate_start();
    write(device_address & ~0x01);
    write(reg_addr);
    for (size_t i = 0; i < len; ++i) {
        write(data[i]);
    }
    generate_stop();
    return true;
}

bool I2C_Driver::read_data(uint8_t reg_addr, uint8_t* buffer, size_t len) {
    generate_start();
    write(device_address & ~0x01);
    write(reg_addr);

    generate_start();
    write(device_address| 0x01);

    for (size_t i = 0; i < len; ++i) {
        buffer[i] = read((i == len - 1) ? AckControl::Disable : AckControl::Enable);
    }
    generate_stop();
    return true;
}

bool I2C_Driver::probe_device() {
    generate_start();
    bool ack = check_event(0x0002);  // ADDR bit in SR1
    generate_stop();
    return ack;
}
