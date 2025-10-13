/*
 * led_display.c
 *
 *  Created on: 10 thg 10, 2025
 *      Author: Trinh Viet Thu
 */

#include "main.h"


void led_mode_display(int mode) {
    switch (mode) {
    case 1: // Mode 1 → LED1 ON
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        break;

    case 2: // Mode 2 → LED2 ON
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        break;

    case 3: // Mode 3 → LED3 ON
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
        break;

    case 4: // Mode 4 → LED4 ON
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
        break;

    default: // safety → all OFF
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
        break;
    }
}
