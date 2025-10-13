/*
 * input_reading.c
 *
 *  Created on: 10 thg 10, 2025
 *      Author: Trinh Viet Thu
 */

#include "main.h"

// we aim to work with more than one buttons
#define N0_OF_BUTTONS 3

// timer interrupt duration = 10ms
// so to detect long press (>1s), count 100 * 10ms = 1s
#define DURATION_FOR_AUTO_INCREASING 100

#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

// debouncing buffers
static GPIO_PinState buttonBuffer[N0_OF_BUTTONS]        = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[N0_OF_BUTTONS]= {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[N0_OF_BUTTONS]= {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

// long-press detection
static uint8_t  flagForButtonPress1s[N0_OF_BUTTONS]     = {0,0,0};
static uint16_t counterForButtonPress1s[N0_OF_BUTTONS]  = {0,0,0};

// define which pins are used for buttons (sw0–sw2)
int GPIO_PIN_BUTTON[N0_OF_BUTTONS] = {GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13};

void button_reading(void) {
    for (int i = 0; i < N0_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_BUTTON[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            buttonBuffer[i] = debounceButtonBuffer1[i];
        }

        if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
            // if button is pressed, start counting
            if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING) {
                counterForButtonPress1s[i]++;
            } else {
                // flag is set when button held >1s
                flagForButtonPress1s[i] = 1;
            }
        } else {
            counterForButtonPress1s[i] = 0;
            flagForButtonPress1s[i] = 0;
        }
    }
}

unsigned char is_button_pressed(uint8_t index) {
    if (index >= N0_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= N0_OF_BUTTONS) return 0xff;
    return (flagForButtonPress1s[index] == 1);
}

