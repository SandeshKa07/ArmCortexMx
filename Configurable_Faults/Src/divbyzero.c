/*
 * divbyzero.c
 *
 *  Created on: May 7, 2021
 *      Author: Sandesh Ashok
 */



#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/*Base address for System handler control status register */

uint32_t *pSHCSR = (uint32_t *)0xE000ED24;

int function_to_cause_div_zero_trap(int x, int y) {
	return x/y;
}

int main(void)
{

	/*Enable the faults in SHCSR */

	*pSHCSR |= (1<<18); /*Usage fault */
	*pSHCSR |= (1<<17); /*Bus fault */
	*pSHCSR |= (1<<16); /*Mem fault */

	/* Enable Div By Zero Trap
	 * If not, then the div by zero is not trapped by processor,
	 * which will return the quotient of 0 */

	uint32_t *divbyzeroTrap = (uint32_t*)0xE000ED14;
	*divbyzeroTrap |= (1<<4);

	/*Call divide by zero */

	function_to_cause_div_zero_trap(1,0);
    /* Loop forever */
	for(;;);
}

void HardFault_Handler(void){
	printf("Hard Fault Handler\n");
	while(1);

}

void MemManage_Handler(void) {
	printf("MemManage Fault Handler\n");
	while(1);

}

void BusFault_Handler(void) {
	printf("Bus Fault Handler\n");
	while(1);

}

__attribute__((naked)) void UsageFault_Handler(void) {
	/* Copy the MSP value to register r4 */
	__asm volatile("MRS r0, MSP");

	/*Branch to the C function */
	__asm volatile("B UsageFault_Handler_c");

}

/* According to Procedure call standards, r0 will be used as the first argument in the callee */

void UsageFault_Handler_c(uint32_t *pBaseStackFrame_from_asm) {

	printf("Usage Fault Handler\n");

	uint32_t *pUFSR = (uint32_t*)0xE000ED2A;
	printf("Fault Status Contents = %lx\n", ((*pUFSR) & 0xFFFF));

	printf("Base Stack Frame Address  = %p\n", pBaseStackFrame_from_asm);
	printf("Value of R0  = %lx\n", pBaseStackFrame_from_asm[0]);
	printf("Value of R1  = %lx\n", pBaseStackFrame_from_asm[1]);
	printf("Value of R2  = %lx\n", pBaseStackFrame_from_asm[2]);
	printf("Value of R3  = %lx\n", pBaseStackFrame_from_asm[3]);
	printf("Value of R12  = %lx\n", pBaseStackFrame_from_asm[4]);
	printf("Value of LR  = %lx\n", pBaseStackFrame_from_asm[5]);
	printf("Value of PC  = %lx\n", pBaseStackFrame_from_asm[6]);
	printf("Value of XPSR  = %lx\n", pBaseStackFrame_from_asm[7]);


	while(1);

}
