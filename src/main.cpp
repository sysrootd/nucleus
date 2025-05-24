#include "stm32.hpp"
#include "gpio.hpp"
#include "adc.hpp"
#include "uart.hpp"
#include "systick.hpp"

int main() {

    //Systick init
    SysTick_Init();
    
    // Set PA1 to analog mode for ADC channel 1
    GPIO gpioC = GPIO(GPIOC);
    gpioC.set_mode(GPIOPin::Pin0, GPIOMode::Analog);

    // Initialize ADC
    ADC1 adc = ADC1(ADC);
    adc.enable();
    adc.configure_channel(10);

    // Initialize UART2
    UART2::init();

    while (true) {
        adc.start_conversion();
        while (!adc.is_conversion_complete());

        uint16_t value = adc.read_value();

        UART2::uart_printf("Temp: %d*C\r\n", value);
        delay_ms(1000);
    }
}
