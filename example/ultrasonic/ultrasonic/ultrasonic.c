#include "ultrasonic.h"
#include "tim.h"

#define CPU_FREQUENCY_MHz 72 // 时钟主频 MHz
#define TIM_HANDLE (&htim4) // Echo TIM 句柄
#define TIM_ADDRESS TIM4 // Echo TIM 地址
#define TIM_CHANNEL TIM_CHANNEL_1 // Echo TIM 通道
#define TIM_COUNTER_PERIOD 65535 // 计数溢出周期
#define DISTANCE_MAXIMUM_CM 450 // 最大测量距离
#define DISTANCE_MINIMUM_CM 2 // 最小测量距离

uint8_t echo_state;
uint16_t tim_overflow_counter;
uint16_t tim_counter_value1;
uint16_t tim_counter_value2;
uint32_t spreading_time_us;
double distance_cm;


void delay_us(__IO uint32_t delay) {
    int last, curr, val;
    int temp;

    while (delay != 0) {
        temp = delay > 900 ? 900 : delay;
        last = SysTick->VAL;
        curr = last - CPU_FREQUENCY_MHz * temp;
        if (curr >= 0) {
            do {
                val = SysTick->VAL;
            } while ((val < last) && (val >= curr));
        } else {
            curr += CPU_FREQUENCY_MHz * 1000;
            do {
                val = SysTick->VAL;
            } while ((val <= last) || (val > curr));
        }
        delay -= temp;
    }
}


void ULTRASONIC_init() {
    /* Start echo input capture */
    HAL_TIM_Base_Start(TIM_HANDLE);
    HAL_TIM_IC_Start_IT(TIM_HANDLE, TIM_CHANNEL);
}

void ULTRASONIC_start() {
    /* generate a 10us pulse */
    HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_SET);
    delay_us(10); // more than 10us
    HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, GPIO_PIN_RESET);

    return;
}

// overload TIM input capture callback function
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    /* Check if interrupt is triggered by the specific timer and channel */
    if (htim->Instance == TIM_ADDRESS) {
        echo_state = HAL_GPIO_ReadPin(Echo_GPIO_Port, Echo_Pin);
        if (echo_state == 1) { // rising edge
            /* reset variables */
            tim_overflow_counter = 0;
            echo_state = 1;

            /* get t1 */
            tim_counter_value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL);

            /* change input capture polarity */
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL, TIM_INPUTCHANNELPOLARITY_FALLING);
        } else if (echo_state == 0) { // falling edge
            tim_counter_value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL);
            spreading_time_us = tim_overflow_counter * TIM_COUNTER_PERIOD + tim_counter_value2 - tim_counter_value1;

            echo_state = 0;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }

    return;
}

// overload TIM overflow interrupt callback function
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    tim_overflow_counter++;

    return;
}

double ULTRASONIC_read() {
    distance_cm = spreading_time_us / 1000000.0 * 340.0 / 2.0 * 100.0;
    if (distance_cm > DISTANCE_MAXIMUM_CM) distance_cm = DISTANCE_MAXIMUM_CM;
    if (distance_cm < DISTANCE_MINIMUM_CM) distance_cm = DISTANCE_MINIMUM_CM;
    return distance_cm;
}