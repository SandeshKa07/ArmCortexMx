/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "led.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Function declaration */
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);
void idle_task(void);
void init_systick_timer(uint32_t tick_hz);
__attribute__((naked)) void init_scheduler_stack(uint32_t scheduler_stack_start);
void init_tasks_dummy_stack(void);
void enable_all_fault_handlers(void);
__attribute__((naked)) void change_sp_to_psp(void);


/* Current task number */
uint32_t current_task = 1; //Task 1 is running

uint32_t global_tick_count = 0;

/* Create a Task Control Block to represent task state */

typedef struct {
	uint32_t psp_value;
	uint32_t block_count;
	uint32_t current_state;
	void (*task_handler)(void);
}TCB_t;


TCB_t user_tasks[MAX_TASKS];


int main(void)
{

	/* Enable all the fault handlers */
	enable_all_fault_handlers();

	/* initialize the Scheduler stack, it used MSP */
	init_scheduler_stack(TASK_SCHED_STACK);

	/* intialize the dummy stack frame for all tasks */
	init_tasks_dummy_stack();

	led_init_all();

	/* initialize the systick timer */
	init_systick_timer(TIME_HZ);

	change_sp_to_psp();

	task1_handler();

	/* Loop forever */
	for(;;);
}


__attribute__((naked)) void init_scheduler_stack(uint32_t scheduler_stack_start){
	/* For function calls, the argument 1 will be held in R0, can be used directly */
	__asm volatile("MSR MSP,R0");
	__asm volatile("BX LR"); //BX copies value of LR to PC

}

uint32_t get_current_task_psp(void){
	return user_tasks[current_task].psp_value;
}

__attribute__((naked)) void change_sp_to_psp(void) {
	__asm volatile("PUSH {LR}");
	__asm volatile("BL get_current_task_psp");
	__asm volatile("MSR PSP, R0");
	__asm volatile("POP {LR}");
	__asm volatile("MOV R0, #0x02");
	__asm volatile("MSR CONTROL, R0");
	__asm volatile("BX LR");// Go back to main function

}


void init_systick_timer(uint32_t tick_hz) {
	/* -1 is, exception will be triggered only when the value is reloaded into downcounter
	 * not when the counter hit zero. So there will an additional count which has to be reduced
	 */
	uint32_t count_value = (SYSTICK_CLOCK / tick_hz) - 1;

	/* Store this count value in Systick Reload Value register */
	uint32_t *pSYST_RVR = (uint32_t*)0xE000E014;

	/* Clear the old value, only 24 bits are used, last 8 bits are reserverd */
	*pSYST_RVR &= ~(0x00FFFFFF);

	/* Save this count value to the register */
	*pSYST_RVR = count_value;

	/*Enable the timer to use processor clock, enable systick exceptions using
	 * Systick Control and Status Register
	 */
	uint32_t *pSYST_CSR = (uint32_t*)0xE000E010;
	*pSYST_CSR |= (1<<2); // Use processor as clock source
	*pSYST_CSR |= (1<<1); // Enable the trigger of exception when clock hits 0.
	*pSYST_CSR |= (1<<0); // Finally enable the systick timer
}


void init_tasks_dummy_stack(void){

	/*Task 0 is always the idle task */
	user_tasks[0].current_state = TASK_RUNNING_STATE;
	user_tasks[1].current_state = TASK_RUNNING_STATE;
	user_tasks[2].current_state = TASK_RUNNING_STATE;
	user_tasks[3].current_state = TASK_RUNNING_STATE;
	user_tasks[4].current_state = TASK_RUNNING_STATE;


	user_tasks[0].psp_value = TASK_IDLE_STACK;
	user_tasks[1].psp_value = TASK_1_STACK;
	user_tasks[2].psp_value = TASK_2_STACK;
	user_tasks[3].psp_value = TASK_3_STACK;
	user_tasks[4].psp_value = TASK_4_STACK;

	user_tasks[0].task_handler = idle_task;
	user_tasks[1].task_handler = task1_handler;
	user_tasks[2].task_handler = task2_handler;
	user_tasks[3].task_handler = task3_handler;
	user_tasks[4].task_handler = task4_handler;

	uint32_t *pPSP_of_task;

	for(int iter = 0; iter < MAX_TASKS; iter ++) {
		pPSP_of_task = (uint32_t*)user_tasks[iter].psp_value;

		/* Full Descending, so Decrement the task pointer first, then update the value */
		pPSP_of_task--;
		*pPSP_of_task = DUMMY_XPSR; // 0x01000000

		pPSP_of_task--;
		*pPSP_of_task = (uint32_t) user_tasks[iter].task_handler;// PC of each stack

		pPSP_of_task--;
		*pPSP_of_task = 0xFFFFFFFD; // Link registers for each task

		/* Initialize the general purpose register to 0 */
		for(int j = 0; j < 13; j++) {
			pPSP_of_task--;
			*pPSP_of_task = 0;
		}

		user_tasks[iter].psp_value = (uint32_t)pPSP_of_task;

	}
}

void enable_all_fault_handlers(void){
	uint32_t *pSCR = (uint32_t*)0xE000ED24;

	*pSCR |= (1<<16); /*Enable Mem  Fault */
	*pSCR |= (1<<17); /* Enable Bus Fault */
	*pSCR |= (1<<18); /*Enable Usage Fault */
}

void save_psp_value_after_storing(uint32_t current_psp_stack_addr){
	user_tasks[current_task].psp_value = current_psp_stack_addr;
}

void update_next_task(void){

	int state = TASK_BLOCKED_STATE;

	for(int i = 0; i < MAX_TASKS; i++) {
		current_task++;
		current_task %= MAX_TASKS;
		state = user_tasks[current_task].current_state;

		if((state == TASK_RUNNING_STATE) && (current_task != 0))
			break;
	}

		/*All tasks are in blocked state, so execute the idle task */
	if(state != TASK_RUNNING_STATE){
		current_task = 0;
	}

}

void schedule(void){
	/* Pend the PendSV, the scheduler will run */
	uint32_t *pICSR = (uint32_t*)0xE000ED04;

	*pICSR |= (1 << 28);
}

/* Put the calling function to blocked state and trigger the scheduler*/
void task_delay(uint32_t tick_count) {

	INTERRUPT_DISABLE();
	if(current_task) {
		user_tasks[current_task].block_count = global_tick_count + tick_count;
		user_tasks[current_task].current_state = TASK_BLOCKED_STATE;
		schedule();
	}
	INTERRUPT_ENABLE();
}

__attribute__((naked)) void PendSV_Handler(void) {
	/* Context Saving
	 * 1. Get the current running task's PSP value.
	 * 2. Using that PSP value, store SF2 (R4 to R11)
	 * 3. Save the current value of PSP.
	 */

	__asm volatile("MRS R0, PSP");

	__asm volatile("STMDB R0!, {R4-R11}");

	/*Before executing BL instructions, save content of LR */

	__asm volatile("PUSH {LR}");
	__asm volatile("BL save_psp_value_after_storing"); //R0 is used as argument to the functions and we have updated R0 value. Geez !!!!!!!!!


	/* Context Retrieving
	 * 1. Decide which task to run next
	 * 1. Get the PSP value of the task to be executed.
	 * 2. Using that PSP value, retrieve SF2(R4 to R11)
	 *  On exception exit, anyways the SF1 will be restored.
	 * 3. Update the PSP and exit.
	 */

	__asm volatile("BL update_next_task");

	__asm volatile("BL get_current_task_psp"); //Returned value will be in R0.

	__asm volatile("LDMIA R0!, {R4-R11}");

	__asm volatile("MSR PSP, R0");// This is where the task1 is shifted to task2, context switching happens.

	__asm volatile("POP {LR}");

	__asm volatile("BX LR"); /*Manually switch back, because of naked function, prologue and epilogue is not present */
}

void update_global_tick_count(void){
	global_tick_count++;
}

void unblock_task(void) {
	for(int i = 1; i < MAX_TASKS; i++) {

		if(user_tasks[i].current_state != TASK_RUNNING_STATE) {

			if(user_tasks[i].block_count == global_tick_count) {

				user_tasks[i].current_state = TASK_RUNNING_STATE;
			}
		}
	}

}

/* Handler here acts as scheduler, it performs context switching */
void SysTick_Handler(void) {

	/*Update the global tick count*/
	update_global_tick_count();

	/* Unblock the task to run state */
	unblock_task();

	/*Pend the PendSV exception */
	uint32_t *pICSR = (uint32_t*)0xE000ED04;

	*pICSR |= (1 << 28);

}

void HardFault_Handler(void){
	printf("Hard Fault Handler\n");
	while(1);
}

void MemManage_Handler(void) {
	printf("MemMange Fault Handler\n");
	while(1);
}

void BusFault_Handler(void) {
	printf("Bus Fault Handler\n");
	while(1);
}

void UsageFault_Handler(void) {
	printf("Usage Fault Handler\n");
	while(1);
}

void idle_task(void) {
	while(1){
		printf("Idle Task\n");
	}
}

void task1_handler(void) {

	while(1){
		led_on(LED_GREEN);
		task_delay(2000);
		led_off(LED_GREEN);
		task_delay(2000);
	}

}

void task2_handler(void){
	while(1){
		led_on(LED_ORANGE);
		task_delay(1000);
		led_off(LED_ORANGE);
		task_delay(1000);
	}
}

void task3_handler(void){
	while(1){
		led_on(LED_BLUE);
		task_delay(500);
		led_off(LED_BLUE);
		task_delay(500);
	}
}

void task4_handler(void){
	while(1){
		led_on(LED_RED);
		task_delay(250);
		led_off(LED_RED);
		task_delay(250);
	}
}
