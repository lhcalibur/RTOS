#include <arch/armv7/inc/svcall.h>

void svc_switchcontext(void)
{
	__asm__ __volatile__ (

	"\tmov		r0, %0\n"	/* R0: context switch */
	"\tsvc		0\n"						/* Force synchronous SVCall (or Hard Fault) */

	/* We will get here only after the rerturn from the context switch */

	"\tbx		lr\n"
	:: "i" ( SYS_switch_context ):
	);
}

