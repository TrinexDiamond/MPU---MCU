/*
 * Exercise9.h
 *
 *  Created on: Sep 28, 2025
 *      Author: Trinh Viet Thu
 */

#ifndef SRC_EXERCISE9_H_
#define SRC_EXERCISE9_H_

#include "main.h"

#define MAX_LED_MATRIX 8

static uint8_t matrix_buffer[MAX_LED_MATRIX] = {
    0b00111100,  // col0
    0b01111110,  // col1
    0b00001111,  // col2
    0b00001111,  // col3
    0b01111110,  // col4
    0b01111110,  // col5
    0b01100110,  // col6
    0b01100110   // col7
};

static inline void cols_off(void) {
    HAL_GPIO_WritePin(enm0_GPIO_Port, enm0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(enm1_GPIO_Port, enm1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(enm2_GPIO_Port, enm2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(enm3_GPIO_Port, enm3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(enm4_GPIO_Port, enm4_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(enm5_GPIO_Port, enm5_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(enm6_GPIO_Port, enm6_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(enm7_GPIO_Port, enm7_Pin, GPIO_PIN_SET);
}

static inline void set_rows(uint8_t bits) {
    HAL_GPIO_WritePin(row0_GPIO_Port, row0_Pin, (bits & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(row1_GPIO_Port, row1_Pin, (bits & 0x02) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(row2_GPIO_Port, row2_Pin, (bits & 0x04) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(row3_GPIO_Port, row3_Pin, (bits & 0x08) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(row4_GPIO_Port, row4_Pin, (bits & 0x10) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(row5_GPIO_Port, row5_Pin, (bits & 0x20) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(row6_GPIO_Port, row6_Pin, (bits & 0x40) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(row7_GPIO_Port, row7_Pin, (bits & 0x80) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

static inline void col_on(int idx) {
    switch (idx) {
        case 0: HAL_GPIO_WritePin(enm0_GPIO_Port, enm0_Pin, GPIO_PIN_RESET); break;
        case 1: HAL_GPIO_WritePin(enm1_GPIO_Port, enm1_Pin, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(enm2_GPIO_Port, enm2_Pin, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(enm3_GPIO_Port, enm3_Pin, GPIO_PIN_RESET); break;
        case 4: HAL_GPIO_WritePin(enm4_GPIO_Port, enm4_Pin, GPIO_PIN_RESET); break;
        case 5: HAL_GPIO_WritePin(enm5_GPIO_Port, enm5_Pin, GPIO_PIN_RESET); break;
        case 6: HAL_GPIO_WritePin(enm6_GPIO_Port, enm6_Pin, GPIO_PIN_RESET); break;
        case 7: HAL_GPIO_WritePin(enm7_GPIO_Port, enm7_Pin, GPIO_PIN_RESET); break;
        default: break;
    }
}

static inline void updateLEDMatrix(int index) {
    if (index < 0 || index >= MAX_LED_MATRIX) return;
    cols_off();
    set_rows(matrix_buffer[index]);
    col_on(index);
}

static inline void refresh_matrix_once(void) {
    for (int i = 0; i < MAX_LED_MATRIX; i++) {
        cols_off();
        set_rows(matrix_buffer[i]);
        col_on(i);
        HAL_Delay(1);
    }
    cols_off();
}


#endif /* SRC_EXERCISE9_H_ */
