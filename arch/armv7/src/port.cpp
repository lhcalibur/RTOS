#include <arch/armv7/inc/port.h>


Port::Port()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	NVIC_SetPriority(SVCall_IRQn, NVIC_SYSH_SVCALL_PRIORITY);
	//NVIC_EnableIRQ(SVCall_IRQn);
	//NVIC_EnableIRQ(SysTick_IRQn);
}
