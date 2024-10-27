#ifndef MAIN_H
#define MAIN_H

// Tao mot enum cac trang thai cua chan echo
typedef enum
{
    HCSR04_IDLE_STATE,         // trang thai khi chua lam gi ca
    HCSR04_WAIT_RAISING_STATE, // trang thai doi xung len
    HCSR04_WAIT_FALLING_STATE, // trang thai doi xung xuong
    HCSR04_COMPLETE_STATE      // trang thai khi hoan thanh tinh toan khoang cach
} HCSR04_State;

// dinh nghia cac chan cua STM32 noi voi HCSR04
#define TRIGGER_PIN GPIOA, GPIO_PIN_0
#define ECHO_PIN GPIOA, GPIO_PIN_1

// khai bao cac ham
void HCSR04_Start();
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HCSR04_Complete_Callback(float kc);
void HCSR04_Handle();

#endif