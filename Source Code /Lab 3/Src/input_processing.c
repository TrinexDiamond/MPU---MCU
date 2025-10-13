/*
 * input_processing.c
 *
 *  Created on: 10 thg 10, 2025
 *      Author: Trinh Viet Thu
 */

#include "main.h"
#include "input_reading.h"
#include "led_display.h"
#include "timer.h"

int mode = 1;
int previousMode;

int color_duration[3] = {5, 2, 3};
int temp_duration[3] = {0, 0, 0};
int set_status[3] = {1, 1, 1};
enum color {red, amber, green};

enum ButtonState { BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_ONE_SECOND };
enum ButtonState buttonState = BUTTON_RELEASED;
enum ButtonState incrementState = BUTTON_RELEASED;
enum ButtonState setState = BUTTON_RELEASED;

void run_led(){
	led_mode_display(mode);
}

typedef enum { RED, AMBER, GREEN } TrafficState;

typedef struct {
    TrafficState state;
    int timer;
    int redPin;
    int yellowPin;
    int greenPin;
} TrafficLight;

TrafficLight light1;
TrafficLight light2;

int initialized = 0;

void initTrafficLights(void) {
    light1.state = RED;
    light1.timer = color_duration[red];
    light1.redPin = GPIO_PIN_1;
    light1.yellowPin = GPIO_PIN_2;
    light1.greenPin = GPIO_PIN_3;

    light2.state = GREEN;
    light2.timer = color_duration[green];
    light2.redPin = GPIO_PIN_4;
    light2.yellowPin = GPIO_PIN_5;
    light2.greenPin = GPIO_PIN_6;

}

void updateTrafficLight(TrafficLight *light) {
    if (light->timer > 1) {
        light->timer--;
        return;
    }

    switch (light->state) {
        case RED:
            light->state = GREEN;
            light->timer = color_duration[green];
            break;
        case AMBER:
            light->state = RED;
            light->timer = color_duration[red];
            break;
        case GREEN:
            light->state = AMBER;
            light->timer = color_duration[amber];
            break;
    }
}

void applyTrafficLight(TrafficLight *light) {
    switch (light->state) {
        case RED:
            HAL_GPIO_WritePin(GPIOA, light->redPin, GPIO_PIN_SET); // RED ON
            HAL_GPIO_WritePin(GPIOA, light->yellowPin, GPIO_PIN_RESET);   // YELLOW OFF
            HAL_GPIO_WritePin(GPIOA, light->greenPin, GPIO_PIN_RESET);   // GREEN OFF
            break;
        case AMBER:
            HAL_GPIO_WritePin(GPIOA, light->redPin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, light->yellowPin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOA, light->greenPin, GPIO_PIN_RESET);
            break;
        case GREEN:
            HAL_GPIO_WritePin(GPIOA, light->redPin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, light->yellowPin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA, light->greenPin, GPIO_PIN_SET);
            break;
    }
}

uint8_t seg_digits[10] = {
	0b11000000, //0
	0b11111001, //1
	0b10100100, //2
	0b10110000, //3
	0b10011001, //4
	0b10010010, //5
	0b10000010, //6
	0b11111000, //7
	0b10000000, //8
	0b10010000  //9
};

void display7SEG(int firstPin, int num){
	if ( num > 9 ) return;
	uint8_t pattern = seg_digits[num];

	for ( int i = 0; i < 7; i++){
		HAL_GPIO_WritePin(GPIOB, firstPin << i, (pattern & (1 << i)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}

int index_led_1 = 0;
int index_led_2 = 0;
int led_1_buffer[2] = {0, 0};
int led_2_buffer[2] = {0, 0};

void update7SEG_led_1(int index){
    // both digits off (active-low PNP → SET = off)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7 | GPIO_PIN_8, GPIO_PIN_SET);

    if (index == 0){
        display7SEG(GPIO_PIN_0, led_1_buffer[0]);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // left ON
    } else {
        display7SEG(GPIO_PIN_0, led_1_buffer[1]);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET); // right ON
    }
}

void update7SEG_led_2(int index){
    // BLANK both top digits first (PNP high-side → SET = OFF)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_SET);

    if (index == 0){
        // left/top digit (tens)
        display7SEG(GPIO_PIN_7, led_2_buffer[0]);   // segments on PB7..PB13
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);   // enable left (active-low)
    } else {
        // right/top digit (units)
        display7SEG(GPIO_PIN_7, led_2_buffer[1]);   // segments on PB7..PB13
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);  // enable right (active-low)
    }
}

void update_led_1_buffer(int num){
	led_1_buffer[0] = num/10;
	led_1_buffer[1] = num%10;
}

void update_led_2_buffer(int num){
	led_2_buffer[0] = num/10;
	led_2_buffer[1] = num%10;
}

void stopTrafficLight(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1 |GPIO_PIN_2 |GPIO_PIN_3 |
							 GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, RESET);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10, SET);
}

void fsm_change_duration(int index){
	switch (incrementState) {
	        case BUTTON_RELEASED:
	            if (is_button_pressed(1)) {
	                incrementState = BUTTON_PRESSED;
	                // INCREASE VALUE OF PORT A BY ONE UNIT
	            }
	            break;

	        case BUTTON_PRESSED:

	            if (!is_button_pressed(1)) {
	            	incrementState = BUTTON_RELEASED;
	            	temp_duration[index]++;
					if (temp_duration[index] > 99) temp_duration[index] = 1;
					set_status[index] = 0;

	            } else {
	                if (is_button_pressed_1s(1)) {
	                    incrementState = BUTTON_PRESSED_MORE_THAN_ONE_SECOND;
	                }
	            }
	            break;

	        case BUTTON_PRESSED_MORE_THAN_ONE_SECOND:
	            if (!is_button_pressed(1)) {
	                incrementState = BUTTON_RELEASED;
	            }
	            // todo

	            if (timer4_flag == 1){
					temp_duration[index]++;
					if (temp_duration[index] > 99) temp_duration[index] = 1;
					set_status[index] = 0;
					setTimer4(100);
	            }
	            break;
	    }
}

void fsm_set_duration(int index){
    switch (setState) {
        case BUTTON_RELEASED:
            if (is_button_pressed(2)) {
                setState = BUTTON_PRESSED;
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(2)) {
                if (temp_duration[index] == 0) temp_duration[index] = 1;
                color_duration[index] = temp_duration[index];
                set_status[index] = 1;
                setState = BUTTON_RELEASED;
            }
            break;

        default:
            setState = BUTTON_RELEASED;
            break;
    }
}

void run_selected_mode(){
	if (mode == 1){
		if (initialized != 1){
			initTrafficLights();
			initialized = 1;
		}
		if (timer0_flag == 1){

			applyTrafficLight(&light1);
			applyTrafficLight(&light2);

			update_led_1_buffer(light1.timer);  // bottom pair

			// top pair (for the other street)
			if (light2.timer < 10) {
			    led_2_buffer[0] = 0;
			    led_2_buffer[1] = light2.timer;
			} else {
			    led_2_buffer[0] = light2.timer / 10;
			    led_2_buffer[1] = light2.timer % 10;
			}

			updateTrafficLight(&light1);
			updateTrafficLight(&light2);

			setTimer0(1000);
		}

		if (timer1_flag == 1){


			update7SEG_led_1(index_led_1);
			update7SEG_led_2(index_led_2);

			index_led_1++;
			index_led_2++;

			if (index_led_1 >= 2) index_led_1 = 0;
			if (index_led_2 >= 2) index_led_2 = 0;
			setTimer1(150);
		}
	}

	if (mode == 2){
		if (initialized == 1 || previousMode == 1){
			initialized = 0;
			previousMode = mode;
			temp_duration[red] = color_duration[red];
			set_status[red] = 0;
			stopTrafficLight();
		}

		if (timer2_flag == 1){
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1|GPIO_PIN_4);
			setTimer2(500);
		}


		update_led_1_buffer(mode);
		if (timer3_flag == 1){
			update7SEG_led_1(index_led_1);
			index_led_1++;
			if (index_led_1 >= 2) index_led_1 = 0;

			update_led_2_buffer(temp_duration[red]);

			update7SEG_led_2(index_led_2);
			index_led_2++;
			if (index_led_2 >= 2) index_led_2 = 0;
			setTimer3(200);
		}

		fsm_change_duration(red);
		fsm_set_duration(red);
	}

	if (mode == 3){
		if (initialized == 1 || previousMode == 2){
			initialized = 0;
			previousMode = mode;
			temp_duration[amber] = color_duration[amber];
			set_status[amber] = 0;
			stopTrafficLight();
		}

		if (timer2_flag == 1){
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2|GPIO_PIN_5);
			setTimer2(500);
		}

		update_led_1_buffer(mode);
		if (timer3_flag == 1){
			update7SEG_led_1(index_led_1);
			index_led_1++;
			if (index_led_1 >= 2) index_led_1 = 0;

			update_led_2_buffer(temp_duration[amber]);

			update7SEG_led_2(index_led_2);
			index_led_2++;
			if (index_led_2 >= 2) index_led_2 = 0;
			setTimer3(200);
		}

		fsm_change_duration(amber);
		fsm_set_duration(amber);
	}

	if (mode == 4){
		if (initialized == 1 || previousMode == 3){
			initialized = 0;
			previousMode = mode;
			temp_duration[green] = color_duration[green];
			stopTrafficLight();
			set_status[green] = 0;
		}

		if (timer2_flag == 1){
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3|GPIO_PIN_6);
			setTimer2(500);
		}

		update_led_1_buffer(mode);
		if (timer3_flag == 1){
			update7SEG_led_1(index_led_1);
			index_led_1++;
			if (index_led_1 >= 2) index_led_1 = 0;

			update_led_2_buffer(temp_duration[green]);

			update7SEG_led_2(index_led_2);
			index_led_2++;
			if (index_led_2 >= 2) index_led_2 = 0;
			setTimer3(200);
		}

		fsm_change_duration(green);
		fsm_set_duration(green);
	}
}

void fsm_for_input_processing(void) {
    switch (buttonState) {
        case BUTTON_RELEASED:
            if (is_button_pressed(0)) {
                buttonState = BUTTON_PRESSED;
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(0)) {
                // short press detected → change mode once
                mode++;
                previousMode = mode - 1;
                if (mode > 4) {
                    mode = 1;
                    previousMode = 4;
                }

                buttonState = BUTTON_RELEASED;  // back to idle
            }
            // (ignore long press — no need for BUTTON_PRESSED_MORE_THAN_1_SECOND)
            break;

        default:
            buttonState = BUTTON_RELEASED;
            break;
    }

    run_led();
    run_selected_mode();
}
