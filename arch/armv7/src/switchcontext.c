#include <arch/armv7/inc/svcall.h>

void svc_switchcontext(void)
{
	__asm__ __volatile__ (

	"\tmov		r0, %0\n"	
	"\tsvc 0	\n"						
	"\tnop		\n"

	"\tbx		lr\n"
	:: "i" ( SYS_switch_context ):
	);
}

