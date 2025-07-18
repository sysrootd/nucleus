#ifndef I2C_DRIVER_HPP
#define I2C_DRIVER_HPP

#include "stm32.hpp"
#include <cstddef>
#include <cstdint>

#define FLAG_MASK ((uint32_t)0x00FFFFFF) // Standard STM32 I2C flag mask

class I2C_Driver {
public:
  enum class SpeedMode {
    Standard, // 100kHz
    Fast      // 400kHz
  };

  enum class AckControl { Disable, Enable };

  I2C_Driver(I2C_Type *i2c, uint8_t device_address);

  void init(SpeedMode mode);
  void enable();
  void disable();
  void start();
  void stop();
  void write(uint8_t data);
  uint8_t read(AckControl ack);
  bool is_busy();
  bool check_event(uint32_t event);
  void wait_for_flag(uint32_t flag);
  void generate_start();
  void generate_stop();

  bool write_register(uint8_t reg_addr, uint8_t data);
  bool read_register(uint8_t reg_addr, uint8_t &data);
  bool write_data(uint8_t reg_addr, const uint8_t *data, size_t len);
  bool read_data(uint8_t reg_addr, uint8_t *buffer, size_t len);
  bool probe_device();

private:
  I2C_Type *i2c_;
  uint8_t device_address;
};

#endif
