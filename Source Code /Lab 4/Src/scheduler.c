/*
 * scheduler.c
 *
 *  Created on: Oct 25, 2025
 *      Author: Trinh Viet Thu
 */

#include "scheduler.h"

static sTask SCH_tasks_G[SCH_MAX_TASKS];
static uint8_t currentTasks = 0;
uint32_t Error_code_G = 0;

// Initialize the scheduler
void SCH_Init(void) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].delay = 0;
        SCH_tasks_G[i].period = 0;
        SCH_tasks_G[i].runMe = 0;
        SCH_tasks_G[i].isEmpty = true;
        SCH_tasks_G[i].isOneShot = false;
    }
    currentTasks = 0;

    HAL_GPIO_WritePin(led05s_GPIO_Port, led05s_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(led10s_GPIO_Port, led10s_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(led15s_GPIO_Port, led15s_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(led20s_GPIO_Port, led20s_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(led25s_GPIO_Port, led25s_Pin, GPIO_PIN_RESET);
}

// Add a new task
uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t delay, uint32_t period) {
    if (currentTasks >= SCH_MAX_TASKS) {
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        return ERROR_SCH_TOO_MANY_TASKS;
    }

    SCH_tasks_G[currentTasks].pTask = pFunction;
    SCH_tasks_G[currentTasks].delay = delay;
    SCH_tasks_G[currentTasks].period = period;
    SCH_tasks_G[currentTasks].runMe = 0;
    SCH_tasks_G[currentTasks].taskID = currentTasks;
    SCH_tasks_G[currentTasks].isEmpty = false;
    SCH_tasks_G[currentTasks].isOneShot = (period == 0);
    currentTasks++;

    return currentTasks - 1;
}

// Called every 10 ms (from TIM interrupt)
void SCH_Update(void) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks_G[i].isEmpty) continue;

        if (SCH_tasks_G[i].delay == 0) {
            // Task is due
            SCH_tasks_G[i].runMe++;
            // Reload delay for periodic tasks
            if (SCH_tasks_G[i].period > 0) {
                SCH_tasks_G[i].delay = SCH_tasks_G[i].period;
            }
            // For one-shot tasks, leave delay at 0 (will be deleted after dispatch)
        } else {
            // Count down to next run
            SCH_tasks_G[i].delay--;
        }
    }
}

// Dispatch ready tasks
void SCH_Dispatch_Tasks(void) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (!SCH_tasks_G[i].isEmpty && SCH_tasks_G[i].runMe > 0) {
            SCH_tasks_G[i].runMe--;
            (*SCH_tasks_G[i].pTask)();   // execute

            if (SCH_tasks_G[i].isOneShot) {
                SCH_Delete_Task(SCH_tasks_G[i].taskID);
            }
        }
    }
}

// Delete a specific task
uint32_t SCH_Delete_Task(uint32_t taskID) {
    if (taskID >= SCH_MAX_TASKS || SCH_tasks_G[taskID].isEmpty) {
        Error_code_G = ERROR_SCH_INVALID_INDEX;
        return ERROR_SCH_INVALID_INDEX;
    }

    for (int i = taskID; i < SCH_MAX_TASKS - 1; i++) {
        SCH_tasks_G[i] = SCH_tasks_G[i + 1];
        SCH_tasks_G[i].taskID = i;
    }

    SCH_tasks_G[SCH_MAX_TASKS - 1].pTask = 0;
    SCH_tasks_G[SCH_MAX_TASKS - 1].delay = 0;
    SCH_tasks_G[SCH_MAX_TASKS - 1].period = 0;
    SCH_tasks_G[SCH_MAX_TASKS - 1].runMe = 0;
    SCH_tasks_G[SCH_MAX_TASKS - 1].isEmpty = true;
    SCH_tasks_G[SCH_MAX_TASKS - 1].isOneShot = false;
    currentTasks--;
    return 0;
}

// Delete all tasks
void SCH_Delete_All(void) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].delay = 0;
        SCH_tasks_G[i].period = 0;
        SCH_tasks_G[i].runMe = 0;
        SCH_tasks_G[i].isEmpty = true;
        SCH_tasks_G[i].isOneShot = false;
    }
    currentTasks = 0;
}
