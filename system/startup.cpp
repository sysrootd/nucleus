#include <cstdint>

// Memory layout
constexpr uint32_t SRAM_START  = 0x20000000U;
constexpr uint32_t SRAM_SIZE   = 64U * 1024U; // 64KB
constexpr uint32_t SRAM_END    = SRAM_START + SRAM_SIZE;
constexpr uint32_t STACK_START = SRAM_END;

// Symbols from linker
extern "C" {
    extern uint32_t _etext;
    extern uint32_t _sdata;
    extern uint32_t _edata;
    extern uint32_t _la_data;
    extern uint32_t _sbss;
    extern uint32_t _ebss;
    extern uint32_t _sheap;
    extern uint32_t _eheap;
}

// Function prototypes
extern "C" {
    int main(void);
    void RCC_Clock_Config(void);
    void Reset_Handler(void) __attribute__((noreturn));
    void Default_Handler(void) __attribute__((noreturn, nothrow));

    // Core exception handlers
    void NMI_Handler(void)                          __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void HardFault_Handler(void)                    __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void MemManage_Handler(void)                    __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void BusFault_Handler(void)                     __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void UsageFault_Handler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SVC_Handler(void)                          __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DebugMon_Handler(void)                     __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void PendSV_Handler(void)                       __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SysTick_Handler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));

    // External interrupts (STM32F401RBT6, total 82 entries)
    void WWDG_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void PVD_IRQHandler(void)                       __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TAMP_STAMP_IRQHandler(void)                __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void RTC_WKUP_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void FLASH_IRQHandler(void)                     __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void RCC_IRQHandler(void)                       __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void EXTI0_IRQHandler(void)                     __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void EXTI1_IRQHandler(void)                     __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void EXTI2_IRQHandler(void)                     __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void EXTI3_IRQHandler(void)                     __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void EXTI4_IRQHandler(void)                     __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA1_Stream0_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA1_Stream1_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA1_Stream2_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA1_Stream3_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA1_Stream4_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA1_Stream5_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA1_Stream6_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void ADC_IRQHandler(void)                       __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void EXTI9_5_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TIM1_BRK_TIM9_IRQHandler(void)             __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TIM1_UP_TIM10_IRQHandler(void)             __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TIM1_TRG_COM_TIM11_IRQHandler(void)        __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TIM1_CC_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TIM2_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TIM3_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TIM4_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void I2C1_EV_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void I2C1_ER_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void I2C2_EV_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void I2C2_ER_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SPI1_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SPI2_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void USART1_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void USART2_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void EXTI15_10_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void RTC_Alarm_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void OTG_FS_WKUP_IRQHandler(void)               __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA1_Stream7_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SDIO_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void TIM5_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SPI3_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA2_Stream0_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA2_Stream1_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA2_Stream2_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA2_Stream3_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA2_Stream4_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void OTG_FS_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA2_Stream5_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA2_Stream6_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DMA2_Stream7_IRQHandler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void USART6_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void I2C3_EV_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void I2C3_ER_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void FPU_IRQHandler(void)                       __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SPI4_IRQHandler(void)                      __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));

    // Vector table
    __attribute__((section(".isr_vector")))
    const uint32_t vectors[] = {
        STACK_START,
        reinterpret_cast<uint32_t>(Reset_Handler),
        reinterpret_cast<uint32_t>(NMI_Handler),
        reinterpret_cast<uint32_t>(HardFault_Handler),
        reinterpret_cast<uint32_t>(MemManage_Handler),
        reinterpret_cast<uint32_t>(BusFault_Handler),
        reinterpret_cast<uint32_t>(UsageFault_Handler),
        0, 0, 0, 0,
        reinterpret_cast<uint32_t>(SVC_Handler),
        reinterpret_cast<uint32_t>(DebugMon_Handler),
        0,
        reinterpret_cast<uint32_t>(PendSV_Handler),
        reinterpret_cast<uint32_t>(SysTick_Handler),

        // External IRQs
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
        0, 0, 0, 0,
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
        0,
        reinterpret_cast<uint32_t>(EXTI15_10_IRQHandler),
        reinterpret_cast<uint32_t>(RTC_Alarm_IRQHandler),
        reinterpret_cast<uint32_t>(OTG_FS_WKUP_IRQHandler),
        0, 0, 0, 0,
        reinterpret_cast<uint32_t>(DMA1_Stream7_IRQHandler),
        0,
        reinterpret_cast<uint32_t>(SDIO_IRQHandler),
        reinterpret_cast<uint32_t>(TIM5_IRQHandler),
        reinterpret_cast<uint32_t>(SPI3_IRQHandler),
        0, 0, 0, 0,
        reinterpret_cast<uint32_t>(DMA2_Stream0_IRQHandler),
        reinterpret_cast<uint32_t>(DMA2_Stream1_IRQHandler),
        reinterpret_cast<uint32_t>(DMA2_Stream2_IRQHandler),
        reinterpret_cast<uint32_t>(DMA2_Stream3_IRQHandler),
        reinterpret_cast<uint32_t>(DMA2_Stream4_IRQHandler),
        0, 0, 0, 0, 0, 0,
        reinterpret_cast<uint32_t>(OTG_FS_IRQHandler),
        reinterpret_cast<uint32_t>(DMA2_Stream5_IRQHandler),
        reinterpret_cast<uint32_t>(DMA2_Stream6_IRQHandler),
        reinterpret_cast<uint32_t>(DMA2_Stream7_IRQHandler),
        reinterpret_cast<uint32_t>(USART6_IRQHandler),
        reinterpret_cast<uint32_t>(I2C3_EV_IRQHandler),
        reinterpret_cast<uint32_t>(I2C3_ER_IRQHandler),
        0, 0, 0, 0, 0, 0, 0,
        reinterpret_cast<uint32_t>(FPU_IRQHandler),
        0, 0,
        reinterpret_cast<uint32_t>(SPI4_IRQHandler)
    };
}

// Default handler
__attribute__((noreturn, nothrow))
void Default_Handler(void) {
    while (true);
}

// RCC clock setup (internal HSI = 16MHz)
void RCC_Clock_Config(void) {
    constexpr uint32_t RCC_BASE = 0x40023800U;
    volatile uint32_t* RCC_CR   = reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x00);
    volatile uint32_t* RCC_CFGR = reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x08);

    *RCC_CR |= (1 << 0);                // Enable HSI
    while (!(*RCC_CR & (1 << 1)));      // Wait for HSI ready

    *RCC_CFGR &= ~(0x3 << 0);           // Select HSI as SYSCLK
    while ((*RCC_CFGR & (0x3 << 2)) != 0); // Wait for HSI to be SYSCLK
}

// Reset handler
void Reset_Handler(void) {
    RCC_Clock_Config();

    // Copy .data
    uint8_t* dst = reinterpret_cast<uint8_t*>(&_sdata);
    uint8_t* src = reinterpret_cast<uint8_t*>(&_la_data);
    while (dst < reinterpret_cast<uint8_t*>(&_edata)) {
        *dst++ = *src++;
    }

    // Zero .bss
    dst = reinterpret_cast<uint8_t*>(&_sbss);
    while (dst < reinterpret_cast<uint8_t*>(&_ebss)) {
        *dst++ = 0;
    }

    main();

    while (true);
}
