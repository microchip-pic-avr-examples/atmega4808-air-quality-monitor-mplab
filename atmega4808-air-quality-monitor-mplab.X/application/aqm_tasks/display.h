#ifndef DISPLAY_H
#define	DISPLAY_H

#include <stdint.h>

#define DISPLAY_TASK_INTERVAL_FIRST (1500L)
#define DISPLAY_TASK_INTERVAL (100L)

#define MAX_DISPLAY_TASK_COUNTER    (15)                //10 for approx 1 sec
#define RESET_DISPLAY_TASK_COUNTER  (0)

uint32_t DISPLAY_Task(void *payload);

void DisplayON(void);



#endif	/* DISPLAY_H */