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
    // Mark Default_Handler as noreturn and nothrow
    void Default_Handler(void) __attribute__((noreturn, nothrow));

    // Weak aliases for exception handlers (must match attributes of Default_Handler)
    void NMI_Handler(void)               __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void HardFault_Handler(void)        __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void MemManage_Handler(void)        __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void BusFault_Handler(void)         __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void UsageFault_Handler(void)       __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SVC_Handler(void)              __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void DebugMon_Handler(void)         __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void PendSV_Handler(void)           __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    void SysTick_Handler(void)          __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));
    
    // External interrupts
    void USART2_IRQHandler(void)        __attribute__((weak, alias("Default_Handler"), noreturn, nothrow));

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
        0, 0, 0, 0,  // Reserved
        reinterpret_cast<uint32_t>(SVC_Handler),
        reinterpret_cast<uint32_t>(DebugMon_Handler),
        0,  // Reserved
        reinterpret_cast<uint32_t>(PendSV_Handler),
        reinterpret_cast<uint32_t>(SysTick_Handler),

        // IRQs — extend as needed
        reinterpret_cast<uint32_t>(USART2_IRQHandler),
    };
}

//Default handler
__attribute__((noreturn, nothrow))
void Default_Handler(void) {
    while (1);
}

// Basic RCC config to enable HSI (16MHz) as SYSCLK
void RCC_Clock_Config(void) {
    constexpr uint32_t RCC_BASE   = 0x40023800U;
    volatile uint32_t* RCC_CR     = reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x00);
    volatile uint32_t* RCC_CFGR   = reinterpret_cast<volatile uint32_t*>(RCC_BASE + 0x08);

    *RCC_CR |= (1 << 0);               // Enable HSI
    while (!(*RCC_CR & (1 << 1)));     // Wait until HSI is ready

    *RCC_CFGR &= ~(0x3 << 0);          // Select HSI as system clock
    while ((*RCC_CFGR & (0x3 << 2)) != 0x00); // Confirm HSI is used
}

// Reset handler: initialize memory and start main()
void Reset_Handler(void) {
    RCC_Clock_Config();

    // Copy .data section from FLASH to SRAM
    uint8_t* dst = reinterpret_cast<uint8_t*>(&_sdata);
    uint8_t* src = reinterpret_cast<uint8_t*>(&_la_data);
    while (dst < reinterpret_cast<uint8_t*>(&_edata)) {
        *dst++ = *src++;
    }

    // Zero initialize .bss section
    dst = reinterpret_cast<uint8_t*>(&_sbss);
    while (dst < reinterpret_cast<uint8_t*>(&_ebss)) {
        *dst++ = 0;
    }

    // Optional: clear heap if needed
    // uint8_t* heap = reinterpret_cast<uint8_t*>(&_sheap);
    // while (heap < reinterpret_cast<uint8_t*>(&_eheap)) {
    //     *heap++ = 0;
    // }

    // Call main()
    main();

    // If main() ever returns
    while (true);
}

