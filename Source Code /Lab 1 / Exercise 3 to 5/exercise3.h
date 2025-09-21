#ifndef __EXERCISE3_H
#define __EXERCISE3_H

#include "main.h"

// Timers & stages
static int init3 = 0;
static int ver_stage = 0, hor_stage = 0;
static int ver_timer = 0, hor_timer = 0;

// ---------------- Vertical Lights ----------------
// Stage 1: GREEN (3s) → Stage 2: YELLOW (2s) → Stage 3: RED (5s)
static inline void ver_setup_init() {
    ver_stage = 1;
    HAL_GPIO_WritePin(GPIOA, RED_V_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, YELLOW_V_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GREEN_V_Pin, GPIO_PIN_SET);
}
static inline void ver_stage2() {
    ver_stage = 2;
    HAL_GPIO_WritePin(GPIOA, GREEN_V_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, YELLOW_V_Pin, GPIO_PIN_SET);
}
static inline void ver_stage3() {
    ver_stage = 3;
    HAL_GPIO_WritePin(GPIOA, YELLOW_V_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, RED_V_Pin, GPIO_PIN_SET);
}
static inline void ver_stage1() {
    ver_stage = 1;
    HAL_GPIO_WritePin(GPIOA, RED_V_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GREEN_V_Pin, GPIO_PIN_SET);
}

// ---------------- Horizontal Lights ----------------
// Stage 1: RED (5s) → Stage 2: GREEN (3s) → Stage 3: YELLOW (2s)
static inline void hor_setup_init() {
    hor_stage = 1;
    HAL_GPIO_WritePin(GPIOA, RED_H_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, YELLOW_H_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GREEN_H_Pin, GPIO_PIN_RESET);
}
static inline void hor_stage2() {
    hor_stage = 2;
    HAL_GPIO_WritePin(GPIOA, RED_H_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GREEN_H_Pin, GPIO_PIN_SET);
}
static inline void hor_stage3() {
    hor_stage = 3;
    HAL_GPIO_WritePin(GPIOA, GREEN_H_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, YELLOW_H_Pin, GPIO_PIN_SET);
}
static inline void hor_stage1() {
    hor_stage = 1;
    HAL_GPIO_WritePin(GPIOA, YELLOW_H_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, RED_H_Pin, GPIO_PIN_SET);
}

// ---------------- Exercise 3 Run ----------------
static inline void exercise3(void) {
    if (init3 == 0) {
        init3 = 1;
        ver_setup_init();
        hor_setup_init();
    }

    switch (ver_stage) {
        case 1: if (ver_timer >= 3) { ver_stage2(); ver_timer = 0; } break;
        case 2: if (ver_timer >= 2) { ver_stage3(); ver_timer = 0; } break;
        case 3: if (ver_timer >= 5) { ver_stage1(); ver_timer = 0; } break;
    }

    switch (hor_stage) {
        case 1: if (hor_timer >= 5) { hor_stage2(); hor_timer = 0; } break;
        case 2: if (hor_timer >= 3) { hor_stage3(); hor_timer = 0; } break;
        case 3: if (hor_timer >= 2) { hor_stage1(); hor_timer = 0; } break;
    }

    ++ver_timer;
    ++hor_timer;
}

#endif
