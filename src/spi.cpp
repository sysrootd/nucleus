#include "spi.hpp"
#include "stm32.hpp"

void SPI_Driver::init(Mode mode, DataSize dataSize, BaudRate baud) {
    // Enable clock for SPI peripheral
    if (spi_ == SPI1) {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    } else if (spi_ == SPI2) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    }

    // Reset SPI
    spi_->CR1 &= ~SPI_CR1_SPE;  // Disable SPI
    
    // Configure mode
    switch (mode) {
        case Mode::Mode0:
            spi_->CR1 &= ~SPI_CR1_CPOL;
            spi_->CR1 &= ~SPI_CR1_CPHA;
            break;
        case Mode::Mode1:
            spi_->CR1 &= ~SPI_CR1_CPOL;
            spi_->CR1 |= SPI_CR1_CPHA;
            break;
        case Mode::Mode2:
            spi_->CR1 |= SPI_CR1_CPOL;
            spi_->CR1 &= ~SPI_CR1_CPHA;
            break;
        case Mode::Mode3:
            spi_->CR1 |= SPI_CR1_CPOL;
            spi_->CR1 |= SPI_CR1_CPHA;
            break;
    }

    // Configure data size
    set_data_size(dataSize);

    // Configure baud rate
    set_baud_rate(baud);

    // Configure as master
    spi_->CR1 |= SPI_CR1_MSTR;

    // Enable SPI
    spi_->CR1 |= SPI_CR1_SPE;
}

void SPI_Driver::enable() {
    spi_->CR1 |= SPI_CR1_SPE;
}

void SPI_Driver::disable() {
    spi_->CR1 &= ~SPI_CR1_SPE;
}

uint8_t SPI_Driver::transmit_receive(uint8_t data) {
    // Wait for TX buffer empty
    while (!(spi_->SR & SPI_SR_TXE));
    
    // Send data
    *((volatile uint8_t*)&spi_->DR) = data;
    
    // Wait for RX buffer not empty
    while (!(spi_->SR & SPI_SR_RXNE));
    
    // Return received data
    return *((volatile uint8_t*)&spi_->DR);
}

uint16_t SPI_Driver::transmit_receive(uint16_t data) {
    // Wait for TX buffer empty
    while (!(spi_->SR & SPI_SR_TXE));
    
    // Send data
    spi_->DR = data;
    
    // Wait for RX buffer not empty
    while (!(spi_->SR & SPI_SR_RXNE));
    
    // Return received data
    return spi_->DR;
}

void SPI_Driver::set_data_size(DataSize size) {
    spi_->CR1 &= ~SPI_CR1_DFF;  // Clear DFF bit
    
    if (size == DataSize::Bits16) {
        spi_->CR1 |= SPI_CR1_DFF;
    }
}

void SPI_Driver::set_baud_rate(BaudRate rate) {
    spi_->CR1 &= ~SPI_CR1_BR;  // Clear baud rate bits
    
    switch (rate) {
        case BaudRate::Div2:   spi_->CR1 |= (0b000 << 3); break;
        case BaudRate::Div4:   spi_->CR1 |= (0b001 << 3); break;
        case BaudRate::Div8:   spi_->CR1 |= (0b010 << 3); break;
        case BaudRate::Div16:  spi_->CR1 |= (0b011 << 3); break;
        case BaudRate::Div32:  spi_->CR1 |= (0b100 << 3); break;
        case BaudRate::Div64:  spi_->CR1 |= (0b101 << 3); break;
        case BaudRate::Div128: spi_->CR1 |= (0b110 << 3); break;
        case BaudRate::Div256: spi_->CR1 |= (0b111 << 3); break;
    }
}