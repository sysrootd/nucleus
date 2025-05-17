// stm32.h
// STM32F401RBT6 Peripheral Register Definitions (C++ style)

#pragma once

#include <cstdint>

#define __IO volatile

// ===================== Base Addresses =====================
#define PERIPH_BASE       0x40000000UL
#define AHB1PERIPH_BASE   0x40020000UL
#define AHB2PERIPH_BASE   0x50000000UL
#define APB1PERIPH_BASE   0x40000000UL
#define APB2PERIPH_BASE   0x40010000UL

#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800UL)
#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE        (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE        (AHB1PERIPH_BASE + 0x0800UL)
#define SYSCFG_BASE       (APB2PERIPH_BASE + 0x3800UL)
#define EXTI_BASE         (APB2PERIPH_BASE + 0x3C00UL)
#define ADC1_BASE         (APB2PERIPH_BASE + 0x2000UL)
#define USART2_BASE       (APB1PERIPH_BASE + 0x4400UL)
#define STK_BASE          0xE000E010UL

// ===================== Peripheral Register Structs =====================
struct GPIO_Type {
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFRL;
    __IO uint32_t AFRH;
};

struct RCC_Type {
    __IO uint32_t CR;
    __IO uint32_t PLLCFGR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t AHB1RSTR;
    __IO uint32_t AHB2RSTR;
    uint32_t RESERVED0[2];
    __IO uint32_t APB1RSTR;
    __IO uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    __IO uint32_t AHB1ENR;
    __IO uint32_t AHB2ENR;
    uint32_t RESERVED2[2];
    __IO uint32_t APB1ENR;
    __IO uint32_t APB2ENR;
    uint32_t RESERVED3[2];
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
    uint32_t RESERVED4[2];
    __IO uint32_t SSCGR;
    __IO uint32_t PLLI2SCFGR;
};

struct EXTI_Type {
    __IO uint32_t IMR;
    __IO uint32_t EMR;
    __IO uint32_t RTSR;
    __IO uint32_t FTSR;
    __IO uint32_t SWIER;
    __IO uint32_t PR;
};

struct SYSCFG_Type {
    __IO uint32_t MEMRMP;
    __IO uint32_t PMC;
    __IO uint32_t EXTICR[4];
    uint32_t RESERVED[2];
    __IO uint32_t CMPCR;
};

struct USART_Type {
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t BRR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t GTPR;
};

struct STK_Type {
    __IO uint32_t CTRL;
    __IO uint32_t LOAD;
    __IO uint32_t VAL;
    __IO uint32_t CALIB;
};

struct ADC_Type {
    __IO uint32_t SR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMPR1;
    __IO uint32_t SMPR2;
    __IO uint32_t JOFR1;
    __IO uint32_t JOFR2;
    __IO uint32_t JOFR3;
    __IO uint32_t JOFR4;
    __IO uint32_t HTR;
    __IO uint32_t LTR;
    __IO uint32_t SQR1;
    __IO uint32_t SQR2;
    __IO uint32_t SQR3;
    __IO uint32_t JSQR;
    __IO uint32_t JDR1;
    __IO uint32_t JDR2;
    __IO uint32_t JDR3;
    __IO uint32_t JDR4;
    __IO uint32_t DR;
};

// ===================== Peripheral Instances =====================
#define GPIOA      ((GPIO_Type*) GPIOA_BASE)
#define GPIOB      ((GPIO_Type*) GPIOB_BASE)
#define GPIOC      ((GPIO_Type*) GPIOC_BASE)
#define RCC        ((RCC_Type*)  RCC_BASE)
#define EXTI_REGS  ((EXTI_Type*) EXTI_BASE)
#define SYSCFG     ((SYSCFG_Type*) SYSCFG_BASE)
#define USART2     ((USART_Type*) USART2_BASE)
#define STK        ((STK_Type*) STK_BASE)
#define ADC1       ((ADC_Type*) ADC1_BASE)

/*
Example usage:
    GPIOB->MODER |= (1 << 26);     // PB13 output
    GPIOC->IDR & (1 << 8);         // Read PC8
    RCC->AHB1ENR |= (1 << 1);      // Enable GPIOB clock
    EXTI_REGS->IMR |= (1 << 0);    // Enable interrupt for EXTI0
*/
