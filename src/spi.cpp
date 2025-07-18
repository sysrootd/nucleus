#include "spi.hpp"
#include "stm32.hpp"

void SPI_Driver::init(Mode mode, DataSize dataSize, BaudRate baud) {
  // Enable SPI clock
  if (spi_ == SPI1) {
    *(volatile uint32_t *)0x40023844 |=
        (1 << 12); // RCC->APB2ENR |= SPI1EN (bit 12)
  } else if (spi_ == SPI2) {
    *(volatile uint32_t *)0x40023840 |=
        (1 << 14); // RCC->APB1ENR |= SPI2EN (bit 14)
  }

  // Disable SPI
  spi_->CR1 &= ~(1 << 6); // SPE = 0

  // Configure mode
  switch (mode) {
  case Mode::Mode0:         // CPOL = 0, CPHA = 0
    spi_->CR1 &= ~(1 << 1); // CPOL = 0
    spi_->CR1 &= ~(1 << 0); // CPHA = 0
    break;
  case Mode::Mode1:         // CPOL = 0, CPHA = 1
    spi_->CR1 &= ~(1 << 1); // CPOL = 0
    spi_->CR1 |= (1 << 0);  // CPHA = 1
    break;
  case Mode::Mode2:         // CPOL = 1, CPHA = 0
    spi_->CR1 |= (1 << 1);  // CPOL = 1
    spi_->CR1 &= ~(1 << 0); // CPHA = 0
    break;
  case Mode::Mode3:        // CPOL = 1, CPHA = 1
    spi_->CR1 |= (1 << 1); // CPOL = 1
    spi_->CR1 |= (1 << 0); // CPHA = 1
    break;
  }

  // Configure data size
  set_data_size(dataSize);

  // Configure baud rate
  set_baud_rate(baud);

  // Set master mode
  spi_->CR1 |= (1 << 2); // MSTR = 1

  // Enable SPI
  spi_->CR1 |= (1 << 6); // SPE = 1
}

void SPI_Driver::enable() {
  spi_->CR1 |= (1 << 6); // SPE = 1
}

void SPI_Driver::disable() {
  spi_->CR1 &= ~(1 << 6); // SPE = 0
}

uint8_t SPI_Driver::transmit_receive(uint8_t data) {
  while (!(spi_->SR & (1 << 1)))
    ;                                      // Wait until TXE (bit 1) is set
  *((volatile uint8_t *)&spi_->DR) = data; // Write data
  while (!(spi_->SR & (1 << 0)))
    ;                                      // Wait until RXNE (bit 0) is set
  return *((volatile uint8_t *)&spi_->DR); // Read received data
}

uint16_t SPI_Driver::transmit_receive(uint16_t data) {
  while (!(spi_->SR & (1 << 1)))
    ; // TXE
  spi_->DR = data;
  while (!(spi_->SR & (1 << 0)))
    ; // RXNE
  return spi_->DR;
}

void SPI_Driver::set_data_size(DataSize size) {
  if (size == DataSize::Bits16) {
    spi_->CR1 |= (1 << 11); // DFF = 1
  } else {
    spi_->CR1 &= ~(1 << 11); // DFF = 0
  }
}

void SPI_Driver::set_baud_rate(BaudRate rate) {
  spi_->CR1 &= ~(0b111 << 3); // Clear BR[2:0] bits

  switch (rate) {
  case BaudRate::Div2:
    spi_->CR1 |= (0b000 << 3);
    break;
  case BaudRate::Div4:
    spi_->CR1 |= (0b001 << 3);
    break;
  case BaudRate::Div8:
    spi_->CR1 |= (0b010 << 3);
    break;
  case BaudRate::Div16:
    spi_->CR1 |= (0b011 << 3);
    break;
  case BaudRate::Div32:
    spi_->CR1 |= (0b100 << 3);
    break;
  case BaudRate::Div64:
    spi_->CR1 |= (0b101 << 3);
    break;
  case BaudRate::Div128:
    spi_->CR1 |= (0b110 << 3);
    break;
  case BaudRate::Div256:
    spi_->CR1 |= (0b111 << 3);
    break;
  }
}
