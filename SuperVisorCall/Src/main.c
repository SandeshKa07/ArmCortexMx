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

int main(void)
{

	/*Generate the SVC exception */

	__asm ("SVC #5");

    /* Loop forever */
	for(;;);
}

__attribute__((naked)) void SVC_Handler(void) {
	__asm ("MRS R0, MSP");
	__asm ("B SVC_Handler_c");
}


void SVC_Handler_c(uint32_t *pBaseStackAddress){
	printf("Inside SVC Handler C function\n");

	uint8_t *pcaddress = (uint8_t *)pBaseStackAddress[6];
	pcaddress -= 2;

	uint8_t svc_number = *pcaddress;
	printf("SVC address = %d\n", svc_number);
}
