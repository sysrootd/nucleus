#pragma once
#include <cstdint>

// =================== GPIO ===================
struct GPIO_Type {
    volatile uint32_t MODER;    // 0x00
    volatile uint32_t OTYPER;   // 0x04
    volatile uint32_t OSPEEDR;  // 0x08
    volatile uint32_t PUPDR;    // 0x0C
    volatile uint32_t IDR;      // 0x10
    volatile uint32_t ODR;      // 0x14
    volatile uint32_t BSRR;     // 0x18
    volatile uint32_t LCKR;     // 0x1C
    volatile uint32_t AFRL;     // 0x20 (AF[0])
    volatile uint32_t AFRH;     // 0x24 (AF[1])
};

// =================== RCC ===================
struct RCC_Type {
    volatile uint32_t CR;           // 0x00
    volatile uint32_t PLLCFGR;      // 0x04
    volatile uint32_t CFGR;         // 0x08
    volatile uint32_t CIR;          // 0x0C
    volatile uint32_t AHB1RSTR;     // 0x10
    volatile uint32_t AHB2RSTR;     // 0x14
    uint32_t RESERVED0[2];          // 0x18, 0x1C
    volatile uint32_t APB1RSTR;     // 0x20
    volatile uint32_t APB2RSTR;     // 0x24
    uint32_t RESERVED1[2];          // 0x28, 0x2C
    volatile uint32_t AHB1ENR;      // 0x30
    volatile uint32_t AHB2ENR;      // 0x34
    uint32_t RESERVED2[2];          // 0x38, 0x3C
    volatile uint32_t APB1ENR;      // 0x40
    volatile uint32_t APB2ENR;      // 0x44
    // ... add more if needed
};

// =================== USART ===================
struct USART_Type {
    volatile uint32_t SR;      // 0x00
    volatile uint32_t DR;      // 0x04
    volatile uint32_t BRR;     // 0x08
    volatile uint32_t CR1;     // 0x0C
    volatile uint32_t CR2;     // 0x10
    volatile uint32_t CR3;     // 0x14
    volatile uint32_t GTPR;    // 0x18
};

// =================== SPI ===================
struct SPI_Type {
    volatile uint32_t CR1;       // 0x00
    volatile uint32_t CR2;       // 0x04
    volatile uint32_t SR;        // 0x08
    volatile uint32_t DR;        // 0x0C
    volatile uint32_t CRCPR;     // 0x10
    volatile uint32_t RXCRCR;    // 0x14
    volatile uint32_t TXCRCR;    // 0x18
    volatile uint32_t I2SCFGR;   // 0x1C
    volatile uint32_t I2SPR;     // 0x20
};

// =================== I2C ===================
struct I2C_Type {
    volatile uint32_t CR1;       // 0x00
    volatile uint32_t CR2;       // 0x04
    volatile uint32_t OAR1;      // 0x08
    volatile uint32_t OAR2;      // 0x0C
    volatile uint32_t DR;        // 0x10
    volatile uint32_t SR1;       // 0x14
    volatile uint32_t SR2;       // 0x18
    volatile uint32_t CCR;       // 0x1C
    volatile uint32_t TRISE;     // 0x20
    volatile uint32_t FLTR;      // 0x24
};

// =================== ADC ===================
struct ADC_Type {
    volatile uint32_t SR;        // 0x00
    volatile uint32_t CR1;       // 0x04
    volatile uint32_t CR2;       // 0x08
    volatile uint32_t SMPR1;     // 0x0C
    volatile uint32_t SMPR2;     // 0x10
    volatile uint32_t JOFR1;     // 0x14
    volatile uint32_t JOFR2;     // 0x18
    volatile uint32_t JOFR3;     // 0x1C
    volatile uint32_t JOFR4;     // 0x20
    volatile uint32_t HTR;       // 0x24
    volatile uint32_t LTR;       // 0x28
    volatile uint32_t SQR1;      // 0x2C
    volatile uint32_t SQR2;      // 0x30
    volatile uint32_t SQR3;      // 0x34
    volatile uint32_t JSQR;      // 0x38
    volatile uint32_t JDR1;      // 0x3C
    volatile uint32_t JDR2;      // 0x40
    volatile uint32_t JDR3;      // 0x44
    volatile uint32_t JDR4;      // 0x48
    volatile uint32_t DR;        // 0x4C
};

// =================== SYSCFG ===================
struct SYSCFG_Type {
    volatile uint32_t MEMRMP;       // 0x00
    volatile uint32_t PMC;          // 0x04
    volatile uint32_t EXTICR[4];    // 0x08 - 0x14
    volatile uint32_t CMPCR;        // 0x18
};

// =================== EXTI ===================
struct EXTI_Type {
    volatile uint32_t IMR;    // Interrupt mask register
    volatile uint32_t EMR;    // Event mask register
    volatile uint32_t RTSR;   // Rising trigger selection register
    volatile uint32_t FTSR;   // Falling trigger selection register
    volatile uint32_t SWIER;  // Software interrupt event register
    volatile uint32_t PR;     // Pending register
};

// =================== TIM ===================
struct TIM_Type {
    volatile uint32_t CR1;       // 0x00
    volatile uint32_t CR2;       // 0x04
    volatile uint32_t SMCR;      // 0x08
    volatile uint32_t DIER;      // 0x0C
    volatile uint32_t SR;        // 0x10
    volatile uint32_t EGR;       // 0x14
    volatile uint32_t CCMR1;     // 0x18
    volatile uint32_t CCMR2;     // 0x1C
    volatile uint32_t CCER;      // 0x20
    volatile uint32_t CNT;       // 0x24
    volatile uint32_t PSC;       // 0x28
    volatile uint32_t ARR;       // 0x2C
    volatile uint32_t RESERVED;  // 0x30
    volatile uint32_t CCR1;      // 0x34
    volatile uint32_t CCR2;      // 0x38
    volatile uint32_t CCR3;      // 0x3C
    volatile uint32_t CCR4;      // 0x40
    volatile uint32_t DCR;       // 0x48
    volatile uint32_t DMAR;      // 0x4C
};

// =================== BASE ADDRESSES ===================
constexpr uintptr_t PERIPH_BASE         = 0x40000000UL;
constexpr uintptr_t AHB1PERIPH_BASE     = PERIPH_BASE + 0x00020000UL;
constexpr uintptr_t APB1PERIPH_BASE     = PERIPH_BASE + 0x00000000UL;
constexpr uintptr_t APB2PERIPH_BASE     = PERIPH_BASE + 0x00010000UL;

constexpr uintptr_t GPIOA_BASE          = AHB1PERIPH_BASE + 0x0000UL;
constexpr uintptr_t GPIOB_BASE          = AHB1PERIPH_BASE + 0x0400UL;
constexpr uintptr_t GPIOC_BASE          = AHB1PERIPH_BASE + 0x0800UL;
constexpr uintptr_t RCC_BASE            = AHB1PERIPH_BASE + 0x3800UL;

constexpr uintptr_t USART2_BASE         = APB1PERIPH_BASE + 0x4400UL;
constexpr uintptr_t USART6_BASE         = APB2PERIPH_BASE + 0x1400UL;

constexpr uintptr_t SPI1_BASE           = APB2PERIPH_BASE + 0x3000UL;
constexpr uintptr_t SPI2_BASE           = APB1PERIPH_BASE + 0x3800UL;

constexpr uintptr_t I2C1_BASE           = APB1PERIPH_BASE + 0x5400UL;
constexpr uintptr_t I2C2_BASE           = APB1PERIPH_BASE + 0x5800UL;

constexpr uintptr_t ADC_BASE            = APB2PERIPH_BASE + 0x2000UL;

constexpr uintptr_t SYSCFG_BASE         = APB2PERIPH_BASE + 0x3800UL;
constexpr uintptr_t EXTI_BASE_ADDR      = APB2PERIPH_BASE + 0x3C00UL;

constexpr uintptr_t TIM2_BASE           = APB1PERIPH_BASE + 0x0000UL;

// =================== PERIPHERAL POINTERS ===================
#define GPIOA   ((GPIO_Type*) GPIOA_BASE)
#define GPIOB   ((GPIO_Type*) GPIOB_BASE)
#define GPIOC   ((GPIO_Type*) GPIOC_BASE)

#define RCC     ((RCC_Type*) RCC_BASE)

#define USART2  ((USART_Type*) USART2_BASE)
#define USART6  ((USART_Type*) USART6_BASE)

#define SPI1    ((SPI_Type*) SPI1_BASE)
#define SPI2    ((SPI_Type*) SPI2_BASE)

#define I2C1    ((I2C_Type*) I2C1_BASE)
#define I2C2    ((I2C_Type*) I2C2_BASE)

#define ADC1     ((ADC_Type*) ADC_BASE)

#define SYSCFG  ((SYSCFG_Type*) SYSCFG_BASE)

#define EXTI_REGS ((EXTI_Type *) EXTI_BASE_ADDR)

#define TIM2    ((TIM_Type*) TIM2_BASE)