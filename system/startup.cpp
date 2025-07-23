#include <cstdint>

// Symbols provided by the linker script
extern uint32_t _estack;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _la_data;
extern uint32_t _sbss;
extern uint32_t _ebss;

extern "C" int main();
extern "C" void Reset_Handler();

#define WEAK __attribute__((weak))
#define ALIAS(f) __attribute__((weak, alias(#f)))

// Forward declarations of handlers
extern "C" {

// Cortex-M4 core handlers
void Reset_Handler();
void NMI_Handler() ALIAS(Default_Handler);
void HardFault_Handler() ALIAS(Default_Handler);
void MemManage_Handler() ALIAS(Default_Handler);
void BusFault_Handler() ALIAS(Default_Handler);
void UsageFault_Handler() ALIAS(Default_Handler);
void SVC_Handler() ALIAS(Default_Handler);
void DebugMon_Handler() ALIAS(Default_Handler);
void PendSV_Handler() ALIAS(Default_Handler);
void SysTick_Handler() ALIAS(Default_Handler);

// STM32F401RBT6 specific IRQ handlers (IRQ0–81)
void WWDG_IRQHandler() ALIAS(Default_Handler);
void PVD_IRQHandler() ALIAS(Default_Handler);
void TAMP_STAMP_IRQHandler() ALIAS(Default_Handler);
void RTC_WKUP_IRQHandler() ALIAS(Default_Handler);
void FLASH_IRQHandler() ALIAS(Default_Handler);
void RCC_IRQHandler() ALIAS(Default_Handler);
void EXTI0_IRQHandler() ALIAS(Default_Handler);
void EXTI1_IRQHandler() ALIAS(Default_Handler);
void EXTI2_IRQHandler() ALIAS(Default_Handler);
void EXTI3_IRQHandler() ALIAS(Default_Handler);
void EXTI4_IRQHandler() ALIAS(Default_Handler);
void DMA1_Stream0_IRQHandler() ALIAS(Default_Handler);
void DMA1_Stream1_IRQHandler() ALIAS(Default_Handler);
void DMA1_Stream2_IRQHandler() ALIAS(Default_Handler);
void DMA1_Stream3_IRQHandler() ALIAS(Default_Handler);
void DMA1_Stream4_IRQHandler() ALIAS(Default_Handler);
void DMA1_Stream5_IRQHandler() ALIAS(Default_Handler);
void DMA1_Stream6_IRQHandler() ALIAS(Default_Handler);
void ADC_IRQHandler() ALIAS(Default_Handler);
void EXTI9_5_IRQHandler() ALIAS(Default_Handler);
void TIM1_BRK_TIM9_IRQHandler() ALIAS(Default_Handler);
void TIM1_UP_TIM10_IRQHandler() ALIAS(Default_Handler);
void TIM1_TRG_COM_TIM11_IRQHandler() ALIAS(Default_Handler);
void TIM1_CC_IRQHandler() ALIAS(Default_Handler);
void TIM2_IRQHandler() ALIAS(Default_Handler);
void TIM3_IRQHandler() ALIAS(Default_Handler);
void TIM4_IRQHandler() ALIAS(Default_Handler);
void I2C1_EV_IRQHandler() ALIAS(Default_Handler);
void I2C1_ER_IRQHandler() ALIAS(Default_Handler);
void I2C2_EV_IRQHandler() ALIAS(Default_Handler);
void I2C2_ER_IRQHandler() ALIAS(Default_Handler);
void SPI1_IRQHandler() ALIAS(Default_Handler);
void SPI2_IRQHandler() ALIAS(Default_Handler);
void USART1_IRQHandler() ALIAS(Default_Handler);
void USART2_IRQHandler() ALIAS(Default_Handler);
void EXTI15_10_IRQHandler() ALIAS(Default_Handler);
void RTC_Alarm_IRQHandler() ALIAS(Default_Handler);
void OTG_FS_WKUP_IRQHandler() ALIAS(Default_Handler);
void DMA1_Stream7_IRQHandler() ALIAS(Default_Handler);
void SDIO_IRQHandler() ALIAS(Default_Handler);
void TIM5_IRQHandler() ALIAS(Default_Handler);
void SPI3_IRQHandler() ALIAS(Default_Handler);
void DMA2_Stream0_IRQHandler() ALIAS(Default_Handler);
void DMA2_Stream1_IRQHandler() ALIAS(Default_Handler);
void DMA2_Stream2_IRQHandler() ALIAS(Default_Handler);
void DMA2_Stream3_IRQHandler() ALIAS(Default_Handler);
void DMA2_Stream4_IRQHandler() ALIAS(Default_Handler);
void OTG_FS_IRQHandler() ALIAS(Default_Handler);
void DMA2_Stream5_IRQHandler() ALIAS(Default_Handler);
void DMA2_Stream6_IRQHandler() ALIAS(Default_Handler);
void DMA2_Stream7_IRQHandler() ALIAS(Default_Handler);
void USART6_IRQHandler() ALIAS(Default_Handler);
void I2C3_EV_IRQHandler() ALIAS(Default_Handler);
void I2C3_ER_IRQHandler() ALIAS(Default_Handler);
void FPU_IRQHandler() ALIAS(Default_Handler);
void SPI4_IRQHandler() ALIAS(Default_Handler);
}

// Default handler
extern "C" void Default_Handler() {
  while (1)
    ;
}

// Interrupt vector table
__attribute__((section(".isr_vector"))) const uint32_t vector_table[] = {
    reinterpret_cast<uint32_t>(&_estack),
    reinterpret_cast<uint32_t>(Reset_Handler),
    reinterpret_cast<uint32_t>(NMI_Handler),
    reinterpret_cast<uint32_t>(HardFault_Handler),
    reinterpret_cast<uint32_t>(MemManage_Handler),
    reinterpret_cast<uint32_t>(BusFault_Handler),
    reinterpret_cast<uint32_t>(UsageFault_Handler), 0, 0, 0, 0, // Reserved
    reinterpret_cast<uint32_t>(SVC_Handler),
    reinterpret_cast<uint32_t>(DebugMon_Handler),
    0, // Reserved
    reinterpret_cast<uint32_t>(PendSV_Handler),
    reinterpret_cast<uint32_t>(SysTick_Handler),

    // IRQ0–81
    reinterpret_cast<uint32_t>(WWDG_IRQHandler),
    reinterpret_cast<uint32_t>(PVD_IRQHandler),
    reinterpret_cast<uint32_t>(TAMP_STAMP_IRQHandler),
    reinterpret_cast<uint32_t>(RTC_WKUP_IRQHandler),
    reinterpret_cast<uint32_t>(FLASH_IRQHandler),
    reinterpret_cast<uint32_t>(RCC_IRQHandler),
    reinterpret_cast<uint32_t>(EXTI0_IRQHandler),
    reinterpret_cast<uint32_t>(EXTI1_IRQHandler),
    reinterpret_cast<uint32_t>(EXTI2_IRQHandler),
    reinterpret_cast<uint32_t>(EXTI3_IRQHandler),
    reinterpret_cast<uint32_t>(EXTI4_IRQHandler),
    reinterpret_cast<uint32_t>(DMA1_Stream0_IRQHandler),
    reinterpret_cast<uint32_t>(DMA1_Stream1_IRQHandler),
    reinterpret_cast<uint32_t>(DMA1_Stream2_IRQHandler),
    reinterpret_cast<uint32_t>(DMA1_Stream3_IRQHandler),
    reinterpret_cast<uint32_t>(DMA1_Stream4_IRQHandler),
    reinterpret_cast<uint32_t>(DMA1_Stream5_IRQHandler),
    reinterpret_cast<uint32_t>(DMA1_Stream6_IRQHandler),
    reinterpret_cast<uint32_t>(ADC_IRQHandler),
    reinterpret_cast<uint32_t>(EXTI9_5_IRQHandler),
    reinterpret_cast<uint32_t>(TIM1_BRK_TIM9_IRQHandler),
    reinterpret_cast<uint32_t>(TIM1_UP_TIM10_IRQHandler),
    reinterpret_cast<uint32_t>(TIM1_TRG_COM_TIM11_IRQHandler),
    reinterpret_cast<uint32_t>(TIM1_CC_IRQHandler),
    reinterpret_cast<uint32_t>(TIM2_IRQHandler),
    reinterpret_cast<uint32_t>(TIM3_IRQHandler),
    reinterpret_cast<uint32_t>(TIM4_IRQHandler),
    reinterpret_cast<uint32_t>(I2C1_EV_IRQHandler),
    reinterpret_cast<uint32_t>(I2C1_ER_IRQHandler),
    reinterpret_cast<uint32_t>(I2C2_EV_IRQHandler),
    reinterpret_cast<uint32_t>(I2C2_ER_IRQHandler),
    reinterpret_cast<uint32_t>(SPI1_IRQHandler),
    reinterpret_cast<uint32_t>(SPI2_IRQHandler),
    reinterpret_cast<uint32_t>(USART1_IRQHandler),
    reinterpret_cast<uint32_t>(USART2_IRQHandler),
    reinterpret_cast<uint32_t>(EXTI15_10_IRQHandler),
    reinterpret_cast<uint32_t>(RTC_Alarm_IRQHandler),
    reinterpret_cast<uint32_t>(OTG_FS_WKUP_IRQHandler),
    reinterpret_cast<uint32_t>(DMA1_Stream7_IRQHandler),
    reinterpret_cast<uint32_t>(SDIO_IRQHandler),
    reinterpret_cast<uint32_t>(TIM5_IRQHandler),
    reinterpret_cast<uint32_t>(SPI3_IRQHandler),
    reinterpret_cast<uint32_t>(DMA2_Stream0_IRQHandler),
    reinterpret_cast<uint32_t>(DMA2_Stream1_IRQHandler),
    reinterpret_cast<uint32_t>(DMA2_Stream2_IRQHandler),
    reinterpret_cast<uint32_t>(DMA2_Stream3_IRQHandler),
    reinterpret_cast<uint32_t>(DMA2_Stream4_IRQHandler),
    reinterpret_cast<uint32_t>(OTG_FS_IRQHandler),
    reinterpret_cast<uint32_t>(DMA2_Stream5_IRQHandler),
    reinterpret_cast<uint32_t>(DMA2_Stream6_IRQHandler),
    reinterpret_cast<uint32_t>(DMA2_Stream7_IRQHandler),
    reinterpret_cast<uint32_t>(USART6_IRQHandler),
    reinterpret_cast<uint32_t>(I2C3_EV_IRQHandler),
    reinterpret_cast<uint32_t>(I2C3_ER_IRQHandler),
    reinterpret_cast<uint32_t>(FPU_IRQHandler),
    reinterpret_cast<uint32_t>(SPI4_IRQHandler),

    // IRQ 56–81: Reserved or not used
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Reset handler
extern "C" void Reset_Handler() {
  // Copy .data section from flash to RAM
  uint32_t *src = &_la_data;
  uint32_t *dst = &_sdata;
  while (dst < &_edata) {
    *dst++ = *src++;
  }

  // Zero initialize .bss section
  dst = &_sbss;
  while (dst < &_ebss) {
    *dst++ = 0;
  }

  // Call main
  main();

  while (1)
    ;
}
