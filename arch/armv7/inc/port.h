#ifndef ARCH_ARM_PORT_H_
#define ARCH_ARM_PORT_H_

#include "stm32f4xx_nucleo.h"
#include "stm32f4xx_hal.h"
#include <include/types.h>
#include <arch/armv7/inc/config.h>
#include <arch/armv7/inc/exc_return.h>
#include <arch/armv7/inc/irq_cmnvector.h>
#include <arch/armv7/inc/psr.h>
class Port
{
	public:
		/* Alternate register names *************************************************/

#define REG_A1              REG_R0
#define REG_A2              REG_R1
#define REG_A3              REG_R2
#define REG_A4              REG_R3
#define REG_V1              REG_R4
#define REG_V2              REG_R5
#define REG_V3              REG_R6
#define REG_V4              REG_R7
#define REG_V5              REG_R8
#define REG_V6              REG_R9
#define REG_V7              REG_R10
#define REG_SB              REG_R9
#define REG_SL              REG_R10
#define REG_FP              REG_R11
#define REG_IP              REG_R12
#define REG_SP              REG_R13
#define REG_LR              REG_R14
#define REG_PC              REG_R15

		/* The PIC register is usually R10. It can be R9 is stack checking is enabled
		 * or if the user changes it with -mpic-register on the GCC command line.
		 */

#define REG_PIC             REG_R10


		/* This structure represents the return state from a system call */

		struct xcpt_syscall_s
		{
			uint32_t excreturn;   /* The EXC_RETURN value */
			uint32_t sysreturn;   /* The return PC */
		};

		/* The following structure is included in the TCB and defines the complete
		 * state of the thread.
		 */

		struct xcptcontext
		{
#ifndef CONFIG_DISABLE_SIGNALS
			/* The following function pointer is non-zero if there
			 * are pending signals to be processed.
			 */

			void *sigdeliver; /* Actual type is sig_deliver_t */

			/* These are saved copies of LR, PRIMASK, and xPSR used during
			 * signal processing.
			 */

			uint32_t saved_pc;
#ifdef CONFIG_ARMV7M_USEBASEPRI
			uint32_t saved_basepri;
#else
			uint32_t saved_primask;
#endif
			uint32_t saved_xpsr;
#ifdef CONFIG_BUILD_PROTECTED
			uint32_t saved_lr;
#endif

# ifdef CONFIG_BUILD_PROTECTED
			/* This is the saved address to use when returning from a user-space
			 * signal handler.
			 */

			uint32_t sigreturn;

# endif
#endif

			/* The following array holds the return address and the exc_return value
			 * needed to return from each nested system call.
			 */

			uint8_t nsyscalls;
			struct xcpt_syscall_s syscall[CONFIG_SYS_NNEST];


			/* Register save area */

			uint32_t regs[XCPTCONTEXT_REGS];
		};


		Port() {}
		void init();
		static void SystemClock_Config();
		static void Error_Handler();

		/* Get/set the PRIMASK register */
		inline uint8_t getprimask(void)
		{
			uint32_t primask;
			__asm__ __volatile__
				(
				 "\tmrs  %0, primask\n"
				 : "=r" (primask)
				 :
				 : "memory");

			return (uint8_t)primask;
		}

		inline void setprimask(uint32_t primask)
		{
			__asm__ __volatile__
				(
				 "\tmsr primask, %0\n"
				 :
				 : "r" (primask)
				 : "memory");
		}

		/* Get/set the BASEPRI register.  The BASEPRI register defines the minimum
		 * priority for exception processing. When BASEPRI is set to a nonzero
		 * value, it prevents the activation of all exceptions with the same or
		 * lower priority level as the BASEPRI value.
		 */

		inline uint8_t getbasepri(void)
		{
			uint32_t basepri;

			__asm__ __volatile__
				(
				 "\tmrs  %0, basepri\n"
				 : "=r" (basepri)
				 :
				 : "memory");

			return (uint8_t)basepri;
		}

		inline void setbasepri(uint32_t basepri)
		{
			__asm__ __volatile__
				(
				 "\tmsr basepri, %0\n"
				 :
				 : "r" (basepri)
				 : "memory");
		}


		/* Disable IRQs */

		inline void irqdisable(void)
		{
#ifdef CONFIG_ARMV7M_USEBASEPRI
			setbasepri(NVIC_SYSH_DISABLE_PRIORITY);
#else
			__asm__ __volatile__ ("\tcpsid  i\n");
#endif
		}

		/* Save the current primask state & disable IRQs */

		inline irqstate_t irqsave(void)
		{
#ifdef CONFIG_ARMV7M_USEBASEPRI
			uint8_t basepri = getbasepri();
			setbasepri(NVIC_SYSH_DISABLE_PRIORITY);
			return (irqstate_t)basepri;
#else
			unsigned short primask;

			/* Return the current value of primask register and set
			 * bit 0 of the primask register to disable interrupts
			 */

			__asm__ __volatile__
				(
				 "\tmrs    %0, primask\n"
				 "\tcpsid  i\n"
				 : "=r" (primask)
				 :
				 : "memory");

			return primask;
#endif
		}

		/* Enable IRQs */

		inline void irqenable(void)
		{
			setbasepri(0);
			__asm__ __volatile__ ("\tcpsie  i\n");
		}

		/* Restore saved primask state */

		inline void irqrestore(irqstate_t flags)
		{
#ifdef CONFIG_ARMV7M_USEBASEPRI
			setbasepri((uint32_t)flags);
#else
			/* If bit 0 of the primask is 0, then we need to restore
			 * interrupts.
			 */

			__asm__ __volatile__
				(
				 "\ttst    %0, #1\n"
				 "\tbne.n  1f\n"
				 "\tcpsie  i\n"
				 "1:\n"
				 :
				 : "r" (flags)
				 : "memory");
#endif
		}

		/* Get/set IPSR */

		inline uint32_t getipsr(void)
		{
			uint32_t ipsr;
			__asm__ __volatile__
				(
				 "\tmrs  %0, ipsr\n"
				 : "=r" (ipsr)
				 :
				 : "memory");

			return ipsr;
		}

		inline void setipsr(uint32_t ipsr)
		{
			__asm__ __volatile__
				(
				 "\tmsr ipsr, %0\n"
				 :
				 : "r" (ipsr)
				 : "memory");
		}

		/* Get/set CONTROL */

		inline uint32_t getcontrol(void)
		{
			uint32_t control;
			__asm__ __volatile__
				(
				 "\tmrs  %0, control\n"
				 : "=r" (control)
				 :
				 : "memory");

			return control;
		}

		inline void setcontrol(uint32_t control)
		{
			__asm__ __volatile__
				(
				 "\tmsr control, %0\n"
				 :
				 : "r" (control)
				 : "memory");
		}
};

#endif /* ARCH_ARM_PORT_H */
