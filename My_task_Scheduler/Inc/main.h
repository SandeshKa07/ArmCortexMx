/*
 * main.h
 *
 *  Created on: May 10, 2021
 *      Author: Sandesh Ashok
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Maximum number of tasks, should scheduler consider */
#define MAX_TASKS			4

/* Configuring RAM START address, size and END address */
#define SRAM_START 			0x20000000U
#define SRAM_SIZE 			(40 * 1024)
#define SRAM_END 			((SRAM_START) + (SRAM_SIZE))

/* Each task will have 1KB stack size */
#define TASK_STACK_SIZE 	1024

/* Stack start address for each task */
#define TASK_1_STACK 		SRAM_END
#define TASK_2_STACK		((SRAM_END) - (1 * TASK_STACK_SIZE))
#define TASK_3_STACK		((SRAM_END) - (2 * TASK_STACK_SIZE))
#define TASK_4_STACK		((SRAM_END) - (3 * TASK_STACK_SIZE))
#define TASK_SCHED_STACK	((SRAM_END) - (4 * TASK_STACK_SIZE))

/*SYSTICK TIME CONFIGURATION */
#define SYSTICK_CLOCK		9000000U
#define TIME_HZ				9000

#define DUMMY_XPSR			0x01000000

#endif /* MAIN_H_ */
