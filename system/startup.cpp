#include <cstdint>

// Memory layout
constexpr uint32_t SRAM_START  = 0x20000000U;
constexpr uint32_t SRAM_SIZE   = 64U * 1024U; // 64KB
constexpr uint32_t SRAM_END    = SRAM_START + SRAM_SIZE;
constexpr uint32_t STACK_START = SRAM_END;

// Symbols from linker
extern "C" {
    extern uint32_t _sidata;
    extern uint32_t _sdata;
    extern uint32_t _edata;
    extern uint32_t _etext;
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
}

// --------- Core Exception Handlers ---------
extern "C" void NMI_Handler(void)                 __attribute__((weak));
extern "C" void HardFault_Handler(void)           __attribute__((weak));
extern "C" void MemManage_Handler(void)           __attribute__((weak));
extern "C" void BusFault_Handler(void)            __attribute__((weak));
extern "C" void UsageFault_Handler(void)          __attribute__((weak));
extern "C" void SVC_Handler(void)                 __attribute__((weak));
extern "C" void DebugMon_Handler(void)            __attribute__((weak));
extern "C" void PendSV_Handler(void)              __attribute__((weak));
extern "C" void SysTick_Handler(void)             __attribute__((weak));

void NMI_Handler(void)                 { Default_Handler(); }
void HardFault_Handler(void)          { Default_Handler(); }
void MemManage_Handler(void)          { Default_Handler(); }
void BusFault_Handler(void)           { Default_Handler(); }
void UsageFault_Handler(void)         { Default_Handler(); }
void SVC_Handler(void)                { Default_Handler(); }
void DebugMon_Handler(void)           { Default_Handler(); }
void PendSV_Handler(void)             { Default_Handler(); }
void SysTick_Handler(void)            { Default_Handler(); }

// --------- Peripheral IRQ Handlers (STM32F401RBT6) ---------

#define DEFINE_IRQ_HANDLER(name) \
    extern "C" void name(void) __attribute__((weak)); \
    extern "C" void name(void) { Default_Handler(); }

DEFINE_IRQ_HANDLER(WWDG_IRQHandler);
DEFINE_IRQ_HANDLER(PVD_IRQHandler);
DEFINE_IRQ_HANDLER(TAMP_STAMP_IRQHandler);
DEFINE_IRQ_HANDLER(RTC_WKUP_IRQHandler);
DEFINE_IRQ_HANDLER(FLASH_IRQHandler);
DEFINE_IRQ_HANDLER(RCC_IRQHandler);
DEFINE_IRQ_HANDLER(EXTI0_IRQHandler);
DEFINE_IRQ_HANDLER(EXTI1_IRQHandler);
DEFINE_IRQ_HANDLER(EXTI2_IRQHandler);
DEFINE_IRQ_HANDLER(EXTI3_IRQHandler);
DEFINE_IRQ_HANDLER(EXTI4_IRQHandler);
DEFINE_IRQ_HANDLER(DMA1_Stream0_IRQHandler);
DEFINE_IRQ_HANDLER(DMA1_Stream1_IRQHandler);
DEFINE_IRQ_HANDLER(DMA1_Stream2_IRQHandler);
DEFINE_IRQ_HANDLER(DMA1_Stream3_IRQHandler);
DEFINE_IRQ_HANDLER(DMA1_Stream4_IRQHandler);
DEFINE_IRQ_HANDLER(DMA1_Stream5_IRQHandler);
DEFINE_IRQ_HANDLER(DMA1_Stream6_IRQHandler);
DEFINE_IRQ_HANDLER(ADC_IRQHandler);
DEFINE_IRQ_HANDLER(EXTI9_5_IRQHandler);
DEFINE_IRQ_HANDLER(TIM1_BRK_TIM9_IRQHandler);
DEFINE_IRQ_HANDLER(TIM1_UP_TIM10_IRQHandler);
DEFINE_IRQ_HANDLER(TIM1_TRG_COM_TIM11_IRQHandler);
DEFINE_IRQ_HANDLER(TIM1_CC_IRQHandler);
DEFINE_IRQ_HANDLER(TIM2_IRQHandler);
DEFINE_IRQ_HANDLER(TIM3_IRQHandler);
DEFINE_IRQ_HANDLER(TIM4_IRQHandler);
DEFINE_IRQ_HANDLER(I2C1_EV_IRQHandler);
DEFINE_IRQ_HANDLER(I2C1_ER_IRQHandler);
DEFINE_IRQ_HANDLER(I2C2_EV_IRQHandler);
DEFINE_IRQ_HANDLER(I2C2_ER_IRQHandler);
DEFINE_IRQ_HANDLER(SPI1_IRQHandler);
DEFINE_IRQ_HANDLER(SPI2_IRQHandler);
DEFINE_IRQ_HANDLER(USART1_IRQHandler);
DEFINE_IRQ_HANDLER(USART2_IRQHandler);
DEFINE_IRQ_HANDLER(EXTI15_10_IRQHandler);
DEFINE_IRQ_HANDLER(RTC_Alarm_IRQHandler);
DEFINE_IRQ_HANDLER(OTG_FS_WKUP_IRQHandler);
DEFINE_IRQ_HANDLER(DMA1_Stream7_IRQHandler);
DEFINE_IRQ_HANDLER(SDIO_IRQHandler);
DEFINE_IRQ_HANDLER(TIM5_IRQHandler);
DEFINE_IRQ_HANDLER(SPI3_IRQHandler);
DEFINE_IRQ_HANDLER(DMA2_Stream0_IRQHandler);
DEFINE_IRQ_HANDLER(DMA2_Stream1_IRQHandler);
DEFINE_IRQ_HANDLER(DMA2_Stream2_IRQHandler);
DEFINE_IRQ_HANDLER(DMA2_Stream3_IRQHandler);
DEFINE_IRQ_HANDLER(DMA2_Stream4_IRQHandler);
DEFINE_IRQ_HANDLER(OTG_FS_IRQHandler);
DEFINE_IRQ_HANDLER(DMA2_Stream5_IRQHandler);
DEFINE_IRQ_HANDLER(DMA2_Stream6_IRQHandler);
DEFINE_IRQ_HANDLER(DMA2_Stream7_IRQHandler);
DEFINE_IRQ_HANDLER(USART6_IRQHandler);
DEFINE_IRQ_HANDLER(I2C3_EV_IRQHandler);
DEFINE_IRQ_HANDLER(I2C3_ER_IRQHandler);
DEFINE_IRQ_HANDLER(FPU_IRQHandler);
DEFINE_IRQ_HANDLER(SPI4_IRQHandler);

// --------- Vector Table ---------

extern "C" __attribute__((section(".isr_vector")))
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

    // External IRQs (per STM32F401RBT6)
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
    0, 0, 0, 0, // Reserved
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

// --------- Default Handler ---------
extern "C" void Default_Handler(void) {
    while (true) {}
}

// --------- RCC Clock Config ---------
void RCC_Clock_Config(void) {
    constexpr uint32_t RCC_BASE = 0x40023800U;
    volatile uint32_t* RCC_CR   = reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x00);
    volatile uint32_t* RCC_CFGR = reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x08);

    *RCC_CR |= (1 << 0);                // Enable HSI
    while (!(*RCC_CR & (1 << 1)));      // Wait for HSI ready

    *RCC_CFGR &= ~(0x3 << 0);           // Select HSI as SYSCLK
    while ((*RCC_CFGR & (0x3 << 2)) != 0); // Wait for HSI to be SYSCLK
}

// --------- Reset Handler ---------
extern "C" void Reset_Handler(void) {
    RCC_Clock_Config();

    // Copy .data
    uint8_t* dst = reinterpret_cast<uint8_t*>(&_sdata);
    uint8_t* src = reinterpret_cast<uint8_t*>(&_sidata);
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
