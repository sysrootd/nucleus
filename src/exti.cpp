#include "stm32f401xe.h"   // defines IRQn_Type and interrupt numbers
#include "core_cm4.h"      // CMSIS core header using IRQn_Type
#include "exti.hpp"
#include "stm32.hpp"

void EXTI::enable(uint8_t line, EXTI_Trigger trigger) {
    // Enable SYSCFG clock
    RCC->APB2ENR |= (1 << 14); // SYSCFGEN

    // Map PA0..PA15 to EXTI0..15
    uint8_t reg = line / 4;
    uint8_t shift = (line % 4) * 4;
    SYSCFG->EXTICR[reg] &= ~(0xF << shift); // Clear
    SYSCFG->EXTICR[reg] |= (0 << shift);    // Port A = 0b0000

    // Configure trigger type
    if (trigger == EXTI_Trigger::Rising || trigger == EXTI_Trigger::Both)
        EXTI_REGS->RTSR |= (1 << line);
    else
        EXTI_REGS->RTSR &= ~(1 << line);

    if (trigger == EXTI_Trigger::Falling || trigger == EXTI_Trigger::Both)
        EXTI_REGS->FTSR |= (1 << line);
    else
        EXTI_REGS->FTSR &= ~(1 << line);

    // Unmask interrupt line
    EXTI_REGS->IMR |= (1 << line);
}

void EXTI::enable_interrupt(uint8_t line) {
    NVIC_EnableIRQ((IRQn_Type)(EXTI0_IRQn + line)); // Enable EXTIx IRQ in NVIC
}

void EXTI::disable_interrupt(uint8_t line) {
    NVIC_DisableIRQ((IRQn_Type)(EXTI0_IRQn + line));
}
