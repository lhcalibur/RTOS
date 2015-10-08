#include <arch/armv7/inc/port.h>

void exception_common(void)
{

	"\tmrs		r0, ipsr \n"				/* R0=exception number */

	/* Complete the context save */

	/* The EXC_RETURN value tells us whether the context is on the MSP or PSP */

	"\ttst		r14, #EXC_RETURN_PROCESS_STACK \n" /* nonzero if context on process stack */
	beq		1f						/* Branch if context already on the MSP */
	mrs		r1, psp					/* R1=The process stack pointer (PSP) */
	mov     sp, r1					/* Set the MSP to the PSP */

1:
	mov		r2, sp					/* R2=Copy of the main/process stack pointer */
	add		r2, #HW_XCPT_SIZE		/* R2=MSP/PSP before the interrupt was taken */
									/* (ignoring the xPSR[9] alignment bit) */
#ifdef CONFIG_ARMV7M_USEBASEPRI
	mrs		r3, basepri				/* R3=Current BASEPRI setting */
#else
	mrs		r3, primask				/* R3=Current PRIMASK setting */
#endif

#ifdef CONFIG_ARCH_FPU

	/* Save the non-volatile FP registers here.
	 *
	 * This routine is the only point where we can save these registers; either before
	 * or after calling up_doirq.  The compiler is free to use them at any time as long
	 * as they are restored before returning, so we can't assume that we can get at the
	 * true values of these registers in any routine called from here.
	 *
	 * REVISIT: we could do all this saving lazily on the context switch side if we knew
	 * where to put the registers.
	 */

	vstmdb	sp!, {s16-s31}			/* Save the non-volatile FP context */

#endif

	stmdb	sp!, {r2-r11,r14}		/* Save the remaining registers plus the SP/PRIMASK values */

#ifndef CONFIG_ARCH_HIPRI_INTERRUPT
	/* Disable interrupts, select the stack to use for interrupt handling
	 * and call up_doirq to handle the interrupt
	 */

	cpsid	i						/* Disable further interrupts */

#else
	/* Set the BASEPRI register so that further normal interrupts will be
	 * masked.  Nested, high priority may still occur, however.
	 */

	mov		r2, #NVIC_SYSH_DISABLE_PRIORITY
	msr		basepri, r2				/* Set the BASEPRI */
#endif

	/* There are two arguments to up_doirq:
	 *
	 *   R0 = The IRQ number
	 *   R1 = The top of the stack points to the saved state
	 */

	mov		r1, sp

	bl		up_doirq				/* R0=IRQ, R1=register save area on stack */
	mrs		r1, msp					/* Get R1=main stack pointer */

	/* On return from up_doirq, R0 will hold a pointer to register context
	 * array to use for the interrupt return.  If that return value is the same
	 * as current stack pointer, then things are relatively easy.
	 */

	cmp		r0, r1					/* Context switch? */
	beq		2f						/* Branch if no context switch */

	/* We are returning with a pending context switch.  This case is different
	 * because in this case, the register save structure does not lie on the
	 * stack but, rather within a TCB structure.  We'll have to copy some
	 * values to the stack.
	 */

	/* Copy the hardware-saved context to the stack, and restore the software
	 * saved context directly.
	 *
	 * XXX In the normal case, it appears that this entire operation is unnecessary;
	 *     context switch time would be improved if we could work out when the stack
	 *     is dirty and avoid the work...
	 */

	add		r1, r0, #SW_XCPT_SIZE 	/* R1=Address of HW save area in reg array */
	ldmia	r1!, {r4-r11}			/* Fetch eight registers in HW save area */
#ifdef CONFIG_ARCH_FPU
	vldmia	r1!, {s0-s15}			/* Fetch sixteen FP registers in HW save area */
	ldmia	r1, {r2-r3}				/* Fetch FPSCR and Reserved in HW save area */
#endif
	ldr		r1, [r0, #(4*REG_SP)]	/* R1=Value of SP before interrupt */
#ifdef CONFIG_ARCH_FPU
	stmdb	r1!, {r2-r3}			/* Store FPSCR and Reserved on the return stack */
	vstmdb	r1!, {s0-s15}			/* Store sixteen FP registers on the return stack */
#endif
	stmdb	r1!, {r4-r11}			/* Store eight registers on the return stack */
	ldmia	r0!, {r2-r11,r14}		/* Recover R4-R11, r14 + 2 temp values */
#ifdef CONFIG_ARCH_FPU
	vldmia	r0, {s16-s31}			/* Recover S16-S31 */
#endif

	b		3f						/* Re-join common logic */

2:
	/* We are returning with no context switch.  We simply need to "unwind"
	 * the same stack frame that we created at entry.
	 */

	ldmia	r1!, {r2-r11,r14}		/* Recover R4-R11, r14 + 2 temp values */
#ifdef CONFIG_ARCH_FPU
	vldmia  r1!, {s16-s31}			/* Recover S16-S31 */
#endif

3:
	/* The EXC_RETURN value tells us whether we are returning on the MSP or PSP
	 */

	/* The EXC_RETURN value will be 0xfffffff9 (privileged thread) or 0xfffffff1
	 * (handler mode) if the stack is on the MSP.  It can only be on the PSP if
	 * EXC_RETURN is 0xfffffffd (unprivileged thread)
	 */

	mrs		r2, control				/* R2=Contents of the control register */
	tst		r14, #EXC_RETURN_PROCESS_STACK /* nonzero if context on process stack */
	beq		4f						/* Branch if privileged */

	orr		r2, r2, #1				/* Unprivileged mode */
	msr		psp, r1					/* R1=The process stack pointer */
	b		5f
4:
	bic		r2, r2, #1				/* Privileged mode */
	msr		msp, r1					/* R1=The main stack pointer */
5:
	msr		control, r2				/* Save the updated control register */

	/* Restore the interrupt state */

#ifdef CONFIG_ARMV7M_USEBASEPRI
	msr		basepri, r3				/* Restore interrupts priority masking */
#ifndef CONFIG_ARCH_HIPRI_INTERRUPT
	cpsie	i						/* Re-enable interrupts */
#endif

#else
	msr		primask, r3				/* Restore interrupts */
#endif

	/* Always return with R14 containing the special value that will: (1)
	 * return to thread mode, and (2) select the correct stack.
	 */

	bx		r14						/* And return */



