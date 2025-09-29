#ifndef SRC_EXERCISE1_H_
#define SRC_EXERCISE1_H_

#include "main.h"

// Lookup table for common-anode 7SEG
int seg_code[11][7] = {
    {0,0,0,0,0,0,1}, // 0
    {1,0,0,1,1,1,1}, // 1
    {0,0,1,0,0,1,0}, // 2
    {0,0,0,0,1,1,0}, // 3
    {1,0,0,1,1,0,0}, // 4
    {0,1,0,0,1,0,0}, // 5
    {0,1,0,0,0,0,0}, // 6
    {0,0,0,1,1,1,1}, // 7
    {0,0,0,0,0,0,0}, // 8
    {0,0,0,0,1,0,0}, // 9
    {1,1,1,1,1,1,1}  // all OFF
};

// Display a digit
void display7SEG(int num){
    if(num >= 0 && num <= 9){
        for(int i = 0; i < 7; i++){
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 << i, seg_code[num][i]);
        }
    } else {
        for(int i = 0; i < 7; i++){
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 << i, seg_code[10][i]);
        }
    }
}

// Turn off both enables
void resetEnable(){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); // en0 off
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // en1 off
}

// Select one 7SEG
void selectEnable(int idx){
    resetEnable();
    if(idx == 0){
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); // en0 on
    } else if(idx == 1){
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // en1 on
    }
}

// --- Timer interrupt ---
int toggleFlag = 0;
int timerCount = 50; // 50 * 10ms = 500ms

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM2){
        timerCount--;
        if(timerCount <= 0){
            if(toggleFlag == 0){
                selectEnable(0);   // en0 ON
                display7SEG(1);    // show "1"
                toggleFlag = 1;
            } else {
                selectEnable(1);   // en1 ON
                display7SEG(2);    // show "2"
                toggleFlag = 0;
            }
            timerCount = 50; // reset for next 500ms
        }
    }
}

#endif /* SRC_EXERCISE1_H_ */
