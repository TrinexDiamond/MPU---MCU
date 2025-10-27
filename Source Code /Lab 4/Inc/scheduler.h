/*
 * scheduler.h
 *
 *  Created on: Oct 25, 2025
 *      Author: Trinh Viet Thu
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include "stdint.h"
#include "stdbool.h"

#define SCH_MAX_TASKS 10

//Error codes
#define ERROR_SCH_INVALID_INDEX    101
#define ERROR_SCH_TOO_MANY_TASKS   102

//Task structure
typedef struct {
    void (*pTask)(void);   // pointer to task function
    uint32_t delay;        // ticks until run
    uint32_t period;       // ticks between runs
    uint8_t runMe;         // flag to mark ready
    uint32_t taskID;
    bool isEmpty;
    bool isOneShot;
} sTask;

//Global Variables
extern uint32_t Error_code_G;

//Function Prototypes
void SCH_Init(void);
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t delay, uint32_t period);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);
uint32_t SCH_Delete_Task(uint32_t taskID);
void SCH_Delete_All(void);

#endif /* INC_SCHEDULER_H_ */
