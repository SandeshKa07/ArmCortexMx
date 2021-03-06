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

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define USART2_IRQ 38
#define USART3_IRQ 39

int main(void)
{
	uint32_t *NVIC_ISPR = (uint32_t*)0xE000E204;
	*NVIC_ISPR |= (1 << (USART3_IRQ % 32));

	uint32_t *NVIC_ISER = (uint32_t*)0xE000E104;
	*NVIC_ISER |= (1 << (USART3_IRQ % 32));
    /* Loop forever */
	for(;;);
}

void USART3_EXTI28_IRQHandler(void) {
	int j = 0;
}
