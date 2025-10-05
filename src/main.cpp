#include "rtos.hpp"
#include "systick.hpp"

using namespace rtos;

Semaphore sem;

void led_task_1(void*) {
    while (1) {
        sem_wait(sem);
        // toggle GPIO here
        // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        sleep_ms(100);
        sem_post(sem);
        sleep_ms(100);
    }
}
void led_task_2(void*) {
    while (1) {
        sem_wait(sem);
        // toggle another LED
        // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
        sleep_ms(50);
        sem_post(sem);
        sleep_ms(50);
    }
}

int main() {
    SysTick_Init();
    rtos::init();

    sem_init(sem, 1);
    create_thread(led_task_1, nullptr, /*prio*/1);
    create_thread(led_task_2, nullptr, /*prio*/1);

    rtos::start(); // never returns
    while (1) {}
}
