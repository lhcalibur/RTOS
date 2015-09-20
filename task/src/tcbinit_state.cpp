#include <include/types.h>
#include <task/inc/task.h>
#include <arch/armv7/inc/port.h>

int Tcb::init_state()
{
	/* Initialize the initial exception register context structure */
	// further
	//memset(xcp, 0, sizeof(struct xcptcontext));

	xcp.regs[REG_SP] = (uint32_t)adj_stack_ptr;
	/* Save the task entry point (stripping off the thumb bit) */

	xcp.regs[REG_PC]      = (uint32_t)task_start & ~1;

	/* Specify thumb mode */

	xcp.regs[REG_XPSR]    = ARMV7M_XPSR_T;

	/* All tasks start via a stub function in kernel space.  So all
	 * tasks must start in privileged thread mode.  If CONFIG_BUILD_PROTECTED
	 * is defined, then that stub function will switch to unprivileged
	 * mode before transferring control to the user task.
	 */

	xcp.regs[REG_EXC_RETURN] = EXC_RETURN_PRIVTHR;


#if 	defined(CONFIG_ARCH_FPU)

	xcp.regs[REG_FPSCR] = 0; // XXX initial FPSCR should be configurable
	xcp.regs[REG_FPReserved] = 0;

#endif /* CONFIG_ARMV7M_CMNVECTOR && !CONFIG_ARMV7M_LAZYFPU && CONFIG_ARCH_FPU */

	/* Enable or disable interrupts, based on user configuration */

#ifdef CONFIG_SUPPRESS_INTERRUPTS

#ifdef CONFIG_ARMV7M_USEBASEPRI
	xcp.regs[REG_BASEPRI] = NVIC_SYSH_DISABLE_PRIORITY;
#else
	xcp.regs[REG_PRIMASK] = 1;
#endif

#else /* CONFIG_SUPPRESS_INTERRUPTS */

#ifdef CONFIG_ARMV7M_USEBASEPRI
	xcp.regs[REG_BASEPRI] = NVIC_SYSH_PRIORITY_MIN;
#endif

#endif /* CONFIG_SUPPRESS_INTERRUPTS */
	return OK;
}

