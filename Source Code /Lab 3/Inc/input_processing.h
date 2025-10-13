/*
 * input_processing.h
 *
 *  Created on: 10 thg 10, 2025
 *      Author: Trinh Viet Thu
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "main.h"

typedef enum {
	BUTTON_RELEASED,
	BUTTON_PRESSED,
	BUTTON_PRESSED_MORE_THAN_ONE_SECOND
} ButtonState;

typedef enum {
	RED,
	AMBER,
	GREEN
} TrafficState;

typedef enum {
	red,
	amber,
	green
} ColorIndex;

typedef struct{
	TrafficState state;
	int timer;
	int redPin;
	int yellowPin;
	int greenPin;
} TrafficLight;

extern int mode;
extern int color_duration[3];
extern TrafficLight light1;
extern TrafficLight light2;

void initTrafficLights(void);
void updateTrafficLight(TrafficLight *light);
void applyTrafficLight(TrafficLight *light);

void run_led(void);
void fsm_change_duration(int index);
void fsm_set_duration(int index);
void run_selected_mode(void);
void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */
