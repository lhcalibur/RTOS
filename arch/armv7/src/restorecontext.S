#include <arch/armv7/inc/svcall.h>

/************************************************************************************
 * Global Symbols
 ************************************************************************************/

	.syntax	unified
	.thumb
	.file	"restorecontext.S"

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
	.globl	svc_restorecontext
	.type	svc_restorecontext, function
up_switchcontext:

	/* Perform the System call with R0=1, R1=saveregs, R2=restoreregs */

	mov		r1, r0					/* R1: restoreregs */
	mov		r0, #SYS_restore_context	/* R0: context restore */
	svc		0						/* Force synchronous SVCall (or Hard Fault) */

	/* We will get here only after the rerturn from the context switch */

	bx		lr
	.size	svc_restorecontext, .-svc_restorecontext
	.end

