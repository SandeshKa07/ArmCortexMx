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

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define TIMER2_IRQ 28
#define I2C1_IRQ 31

uint32_t *pNVIC_ISER_0 = (uint32_t*)0xE000E100; // NVIC_ISER0 will be used because IRQs are 28 and 32
uint32_t *pNVIC_ISPR_0 = (uint32_t*) 0XE000E200; // NVIC_ISPR0 will be used because IRQs are 28 and 32
uint32_t *pNVIC_IPR = (uint32_t*) 0xE000E400; // Base register of IPR

void configure_ipr_to_irq(uint8_t irq, uint8_t priority_value) {

	/* Find the IPR to be used among 60 available registers */
	uint8_t ipr_num = irq / 4;
	uint32_t *ipr_register = pNVIC_IPR + ipr_num;

	/* Find the priority field to be used, among 4 priority field in each 32 bit register */
	uint8_t ipr_position = (irq % 4) * 8;

	/* Clear the existing priority values */
	*ipr_register &= ~(0xFF << ipr_position);

	/* Set the new priority values */
	*ipr_register |= (priority_value << ipr_position);

}

int main(void)
{
	/* Set the priority register for the interrupt */

	configure_ipr_to_irq(TIMER2_IRQ, 0x80);
	configure_ipr_to_irq(I2C1_IRQ, 0x7F);

	/* Manually Pend the interrupt */
	*pNVIC_ISPR_0 |= (1<< TIMER2_IRQ);

	/* Enbale the interrupt */
	*pNVIC_ISER_0 |= (1<< I2C1_IRQ);
	*pNVIC_ISER_0 |= (1<< TIMER2_IRQ);

    /* Loop forever */
	for(;;);
}

void TIM2_IRQHandler(void){
	printf("Timer 1  interrupt service routine\n");

	*pNVIC_ISPR_0 |= (1<< I2C1_IRQ);

	while(1);

}

void I2C1_EV_EXTI23_IRQHandler(void) {
	printf("I2C1 interrupt service routine\n");
}
