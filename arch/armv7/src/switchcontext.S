#include <arch/armv7/inc/svcall.h>

/************************************************************************************
 * Global Symbols
 ************************************************************************************/

	.syntax	unified
	.thumb
	.file	"switchcontext.S"

/************************************************************************************
 * Macros
 ************************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: up_switchcontext
 *
 * Description:
 *   Save the current thread context and restore the specified context.
 *   Full prototype is:
 *
 *   void up_switchcontext(uint32_t *saveregs, uint32_t *restoreregs);
 *
 * Return:
 *   None
 *
 ************************************************************************************/

	.thumb_func
	.globl	svc_switchcontext
	.type	svc_switchcontext, function
up_switchcontext:

	mov		r0, #SYS_switch_context	/* R0: context switch */
	svc		0						/* Force synchronous SVCall (or Hard Fault) */

	/* We will get here only after the rerturn from the context switch */

	bx		lr
	.size	svc_switchcontext, .-svc_switchcontext
	.end

