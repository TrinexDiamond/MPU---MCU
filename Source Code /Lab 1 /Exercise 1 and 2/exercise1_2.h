#ifndef __EXERCISE1_2_H
#define __EXERCISE1_2_H

#include "main.h"

static inline void Exercise1_2_Run(void) {
    while (1) {
        // RED ON (5s)
        HAL_GPIO_WritePin(GPIOA, LED_RED_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, LED_YELLOW_Pin | LED_GREEN_Pin, GPIO_PIN_RESET);
        HAL_Delay(5000);

        // YELLOW ON (2s)
        HAL_GPIO_WritePin(GPIOA, LED_YELLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, LED_RED_Pin | LED_GREEN_Pin, GPIO_PIN_RESET);
        HAL_Delay(2000);

        // GREEN ON (3s)
        HAL_GPIO_WritePin(GPIOA, LED_GREEN_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, LED_RED_Pin | LED_YELLOW_Pin, GPIO_PIN_RESET);
        HAL_Delay(3000);
    }
}

#endif
