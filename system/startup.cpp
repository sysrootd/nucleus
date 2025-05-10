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
    void Default_Handler(void) __attribute__((noreturn));

    // Weak aliases for system exception handlers
    void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
    void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
    void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
    void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
    void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
    void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
    void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
    void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
    void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

    // Add common IRQs you might use later
    void USART2_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

    // Vector table
    const uint32_t vectors[] __attribute__((section(".isr_vector"))) = {
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
        // External interrupts (fill as needed)
        reinterpret_cast<uint32_t>(USART2_IRQHandler),
    };
}

// Default fault/IRQ handler
void Default_Handler(void) {
    while (true);  // Infinite loop in case of fault
}

// Configure RCC to use HSI as system clock
void RCC_Clock_Config(void) {
    volatile uint32_t* RCC_CR   = reinterpret_cast<uint32_t*>(0x40023800U + 0x00);
    volatile uint32_t* RCC_CFGR = reinterpret_cast<uint32_t*>(0x40023800U + 0x08);

    *RCC_CR |= (1 << 0); // Enable HSI
    while (!(*RCC_CR & (1 << 1))); // Wait for HSI ready

    *RCC_CFGR &= ~(0x3 << 0); // Select HSI
    while ((*RCC_CFGR & (0x3 << 2)) != 0x00); // Wait till active
}

// Reset handler
void Reset_Handler(void) {
    RCC_Clock_Config();

    // Copy .data section
    uint8_t* pDst = reinterpret_cast<uint8_t*>(& _sdata);
    uint8_t* pSrc = reinterpret_cast<uint8_t*>(& _la_data);
    while (pDst < reinterpret_cast<uint8_t*>(& _edata)) {
        *pDst++ = *pSrc++;
    }

    // Zero .bss section
    pDst = reinterpret_cast<uint8_t*>(& _sbss);
    while (pDst < reinterpret_cast<uint8_t*>(& _ebss)) {
        *pDst++ = 0;
    }

    // Optional heap init (skip if not using malloc yet)
    // uint8_t* heap_ptr = reinterpret_cast<uint8_t*>(& _sheap);
    // while (heap_ptr < reinterpret_cast<uint8_t*>(& _eheap)) {
    //     *heap_ptr++ = 0;
    // }

    // Jump to main
    main();

    // Fallback if main ever returns (infinite loop)
    while (true);
}

