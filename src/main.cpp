#include "stm32.hpp"
#include "gpio.hpp"
#include "adc.hpp"
#include "uart.hpp"
#include "systick.hpp"

// Callback function to handle ADC conversion result
void adc_callback(uint16_t value) {
    UART2::uart_printf("ADC Value (Interrupt): %d\r\n", value);
}

int main() {
    // Initialize SysTick (default 16 MHz)
    SysTick_Init();

    // Initialize UART2 for printing
    UART2::init();

    // Configure PC0 (ADC Channel 10) as analog input
    GPIO gpioC = GPIO(GPIOC);
    GPIO gpioA = GPIO(GPIOA)
    gpioC.set_mode(GPIOPin::Pin0, GPIOMode::Analog);

    // Initialize ADC1
    ADC1 adc(ADC);
    adc.enable();
    adc.configure_channel(10);         // Channel 10 = PC0
    adc.set_callback(adc_callback);    // Set ISR callback
    adc.enable_interrupt();            // Enable EOC interrupt

    while (1) {
        adc.start_conversion();        // Start a conversion
        delay_ms(1000);                // Delay for readability
    }
}
