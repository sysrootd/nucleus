#pragma once
#include "stm32.hpp"

template <typename TIMx>
class Timer {
public:
    static void init(uint16_t prescaler, uint32_t autoReload) {
        enableClock();
        TIMx::regs()->PSC = prescaler;
        TIMx::regs()->ARR = autoReload;
        TIMx::regs()->CNT = 0;
        TIMx::regs()->CR1 |= (1 << 0);  // CEN: counter enable
    }

    static void stop() {
        TIMx::regs()->CR1 &= ~(1 << 0);
    }

    static uint32_t count() {
        return TIMx::regs()->CNT;
    }

private:
    static void enableClock();
};

// ========= Specializations for different TIM peripherals =========

// Helper struct to associate TIMx register and RCC bit
struct TIM2_Info {
    static TIM_Type* regs() { return TIM2; }
    static constexpr uint32_t rcc_bit = (1 << 0);
};

// Specialize clock enable for Timer2
template<>
inline void Timer<TIM2_Info>::enableClock() {
    RCC->APB1ENR |= TIM2_Info::rcc_bit;
}

// Comment out or remove TIM3_Info if TIM3 is undefined
/*
struct TIM3_Info {
    static TIM_Type* regs() { return TIM3; }
    static constexpr uint32_t rcc_bit = (1 << 1);
};

template<>
inline void Timer<TIM3_Info>::enableClock() {
    RCC->APB1ENR |= TIM3_Info::rcc_bit;
}
*/
