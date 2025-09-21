#ifndef __EXERCISE4_H
#define __EXERCISE4_H

#include "main.h"

// Segment map for common ANODE (a–g)
// 0 = segment ON, 1 = segment OFF
static const uint8_t seg_map[10][7] = {
    {0,0,0,0,0,0,1}, // 0
    {1,0,0,1,1,1,1}, // 1
    {0,0,1,0,0,1,0}, // 2
    {0,0,0,0,1,1,0}, // 3
    {1,0,0,1,1,0,0}, // 4
    {0,1,0,0,1,0,0}, // 5
    {0,1,0,0,0,0,0}, // 6
    {0,0,0,1,1,1,1}, // 7
    {0,0,0,0,0,0,0}, // 8
    {0,0,0,0,1,0,0}  // 9
};

// Display digit on Display1 (pins B0..B6)
static inline void display7SEG1(int num) {
    if (num < 0 || num > 9) return;
    for (int i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(GPIOB, (GPIO_PIN_0 << i), seg_map[num][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

// Display digit on Display2 (pins B7..B13)
static inline void display7SEG2(int num) {
    if (num < 0 || num > 9) return;
    for (int i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(GPIOB, (GPIO_PIN_7 << i), seg_map[num][i] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

#endif
