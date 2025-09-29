/*
 * Exercise2to8.h
 *
 *  Created on: Sep 27, 2025
 *      Author: Trinh Viet Thu
 */

//Remember to Un-comment exercise 2 and comment exercise 3-8 for exercise 2's schematic to work properly

#ifndef SRC_EXERCISE2TO8_H_
#define SRC_EXERCISE2TO8_H_

#include "main.h"

#define MAX_LED 4

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
    {1,1,1,1,1,1,1}  // OFF
};

void display7SEG(int num){
    if(num < 0 || num > 9) num = 10; // OFF

    HAL_GPIO_WritePin(GPIOB, aseg_Pin, seg_code[num][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, bseg_Pin, seg_code[num][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, cseg_Pin, seg_code[num][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, dseg_Pin, seg_code[num][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, eseg_Pin, seg_code[num][4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, fseg_Pin, seg_code[num][5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, gseg_Pin, seg_code[num][6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void resetEnable(){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); // en0
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // en1
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // en2
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // en3
}

void selectEnable(int idx){
    resetEnable();
    switch(idx){
        case 0: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); break;
        case 1: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); break;
        case 2: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); break;
        case 3: HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET); break;
        default: break;
    }
}

int led_buffer[4] = {1, 2, 3, 0};
int index_led = 0;



//================EXERCISE 6====================
int timer0_counter = 0;
int timer0_flag = 0;
const int TIMER_CYCLE = 10; // 10 ms tick from TIM2

void setTimer0(int duration){
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

//void timer_run(){
//    if(timer0_counter > 0){
//        timer0_counter--;
//        if(timer0_counter == 0){
//            timer0_flag = 1;
//        }
//    }
//}
//==============================================



//================EXERCISE 7 & 8====================
int timer1_counter = 0;
int timer1_flag = 0;
int timer2_counter = 0;
int timer2_flag = 0;


void setTimer1(int duration){
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}
void setTimer2(int duration){
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}

void timer_run(){
    // timer0 (clock tick)
    if(timer0_counter > 0){
        timer0_counter--;
        if(timer0_counter == 0) timer0_flag = 1;
    }
    // timer1 (DOT blink)
    if(timer1_counter > 0){
        timer1_counter--;
        if(timer1_counter == 0) timer1_flag = 1;
    }
    // timer2 (7SEG scan)
    if(timer2_counter > 0){
        timer2_counter--;
        if(timer2_counter == 0) timer2_flag = 1;
    }
}
//==============================================



//================EXERCISE 5====================
int hour = 12, minute = 30, second = 58;

void updateClockBuffer(){
    led_buffer[0] = hour / 10;     // tens of hour
    led_buffer[1] = hour % 10;     // units of hour
    led_buffer[2] = minute / 10;   // tens of minute
    led_buffer[3] = minute % 10;   // units of minute
}
//=============================================



//================EXERCISE 4====================
int clockScan = 25;
//Exercise 2: int clockScan = 50;  // 50 * 10ms = 500ms
int clockDot  = 100; // 100 * 10ms = 1s



//=================Exercise	2===================
//void update7SEG(int index){
//   if(index >= 4) index_led = 0;
//    selectEnable(index);
//    display7SEG(led_buffer[index]);
//}
//==============================================



//=================EXERCISE 3===================
//void update7SEG(int index){
//    switch(index){
//        case 0:
//            selectEnable(0);
//            display7SEG(led_buffer[0]);
//            break;
//        case 1:
//            selectEnable(1);
//            display7SEG(led_buffer[1]);
//            break;
//        case 2:
//            selectEnable(2);
//            display7SEG(led_buffer[2]);
//            break;
//        case 3:
//            selectEnable(3);
//            display7SEG(led_buffer[3]);
//            break;
//        default:
//            break;
//    }
//}
//=============================================

//==================EXERCISE 5=================
void update7SEG(int index){
    if(index >= MAX_LED) index = 0;   // just in case
    selectEnable(index);              // enable one digit
    display7SEG(led_buffer[index]);   // show its value
}

#endif /* SRC_EXERCISE2TO8_H_ */
