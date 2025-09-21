#ifndef __EXERCISE5_H
#define __EXERCISE5_H

#include "main.h"
#include "exercise3.h"
#include "exercise4.h"

static inline void exercise5(void) {
    exercise3(); // update traffic lights

    // Vertical light countdown
    switch (ver_stage) {
        case 1: display7SEG1(3 - ver_timer); break;
        case 2: display7SEG1(2 - ver_timer); break;
        case 3: display7SEG1(5 - ver_timer); break;
    }

    // Horizontal light countdown
    switch (hor_stage) {
        case 1: display7SEG2(5 - hor_timer); break;
        case 2: display7SEG2(3 - hor_timer); break;
        case 3: display7SEG2(2 - hor_timer); break;
    }
}

#endif
