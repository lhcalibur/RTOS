#include <arch/armv7/inc/svcall.h>
#include <arch/armv7/inc/port.h>

	/* Perform the System call with R0=1, R1=saveregs, R2=restoreregs */
void svc_restore_context(uint32_t *regs)
{
	__asm__ __volatile__ (

	"\tmov		r1, r0\n"					/* R1: restoreregs */
	"\tmov		r0, %0\n"	/* R0: context restore */
	"\tsvc		0\n"						/* Force synchronous SVCall (or Hard Fault) */

	/* We will get here only after the rerturn from the context switch */

	"\tbx		lr\n"
	:: "i" ( SYS_restore_context ): "r0", "r1"
	);
}

