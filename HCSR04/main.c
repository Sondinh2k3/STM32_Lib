#include "main.h"

TIM_HandleTypeDef htim1;

HCSR04_State HCSR04_state = HCSR04_IDLE_STATE; // tao mot bien luu trang thai, dau tien chua lam gi ca

// tao mot ham start
void HCSR04_Start()
{
    if (HCSR04_state == HCSR04_IDLE_STATE)
    {
        // De start HCSR04, ta se cap mot xung cho chan TRIGGER
        HAL_GPIO_WritePin(TRIGGER_PIN, GPIO_PIN_SET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(TRIGGER_PIN, GPIO_PIN_RESET);

        // sau khi cap xung cho chan TRIGGER, chung ta se chuyen trang thai sang cho xung len
        HCSR04_state = HCSR04_WAIT_RAISING_STATE;
    }
}

// Ham callback khi xay ra ngat ngoai
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (HCSR04_state)
    {
    case HCSR04_WAIT_RAISING_STATE:
        if (HAL_GPIO_ReadPin(ECHO_PIN) == 1)
        {
            htim1.Instance->CNT = 0;
            HCSR04_state = HCSR04_WAIT_FALLING_STATE;
            HAL_TIM_Base_Start(&htim1);
        }
        else
        {
            HCSR04_state = HCSR04_IDLE_STATE;
        }
        break;

    case HCSR04_WAIT_FALLING_STATE:
        if (HAL_GPIO_ReadPin(ECHO_PIN) == 0)
        {
            HAL_TIM_Base_Stop(&htim1);
            HCSR04_state = HCSR04_COMPLETE_STATE;
        }
        else
        {
            HCSR04_state = HCSR04_IDLE_STATE;
        }
        break;

    default:
        break;
    }
}

// Ham callback khi do xong => ta se xu ly viec can lam khi do xong trong nay
void HCSR04_Complete_Callback(float kc)
{
    // Cho nay la cho nguoi dung xu ly
}

void HCSR04_Handle()
{
    if (HCSR04_state == HCSR04_COMPLETE_STATE)
    {
        // tinh toan khoang cach:
        float kc = 0.017 * htim1.Instance->CNT;
        // Sau khi do xong, ta se goi den ham Callback de xu ly
        HCSR04_Complete_Callback(kc);

        // Sau khi Callback xong ta se gan lai state là Idle => Cho xung tiep theo de do khoang cach tiep
        HCSR04_state = HCSR04_IDLE_STATE;
    }
}

int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/

    while (1)
    {
        // Ta se thuc hien truyen xung de bat dau do sau 300ms mot lan
        uint32_t t_Get_Distance = 0;
        if (HAL_GetTick() - t_Get_Distance >= 300)
        {
            HCSR04_Start();
            t_Get_Distance = HAL_GetTick();
        }
        HCSR04_Handle();
    }
}
