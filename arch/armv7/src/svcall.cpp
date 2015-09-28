#include <include/config.h>
#include <arch/armv7/inc/svcall.h>
#include <arch/armv7/inc/irq_cmnvector.h>
#include <arch/armv7/inc/port.h>
#include <sys/inc/kernel.h>


int up_svcall(int irq, void *context)
{
  uint32_t *regs = (uint32_t*)context;
  uint32_t cmd;

  cmd = regs[REG_R0];

  /* The SVCall software interrupt is called with R0 = system call command
   * and R1..R7 =  variable number of arguments depending on the system call.
   */
  /* Handle the SVCall according to the command in R0 */

  switch (cmd)
    {
      /* R0=SYS_save_context:  This is a save context command:
       *
       *   int up_saveusercontext(uint32_t *saveregs);
       *
       * At this point, the following values are saved in context:
       *
       *   R0 = SYS_save_context
       *   R1 = saveregs
       *
       * In this case, we simply need to copy the current regsters to the
       * save register space references in the saved R1 and return.
       */

      case SYS_save_context:
        {
//          memcpy((uint32_t*)regs[REG_R1], regs, XCPTCONTEXT_SIZE);
        }
        break;

      /* R0=SYS_restore_context:  This a restore context command:
       *
       *   void up_fullcontextrestore(uint32_t *restoreregs) noreturn_function;
       *
       * At this point, the following values are saved in context:
       *
       *   R0 = SYS_restore_context
       *   R1 = restoreregs
       *
       * In this case, we simply need to set current_regs to restore register
       * area referenced in the saved R1. context == current_regs is the normal
       * exception return.  By setting current_regs = context[R1], we force
       * the return to the saved context referenced in R1.
       */

      case SYS_restore_context:
        {
//          current_regs = (uint32_t*)regs[REG_R1];
        }
        break;

      /* R0=SYS_switch_context:  This a switch context command:
       *
       *   void up_switchcontext(uint32_t *saveregs, uint32_t *restoreregs);
       *
       * At this point, the following values are saved in context:
       *
       *   R0 = SYS_switch_context
       *   R1 = saveregs
       *   R2 = restoreregs
       *
       * In this case, we do both: We save the context registers to the save
       * register area reference by the saved contents of R1 and then set
       * current_regs to to the save register area referenced by the saved
       * contents of R2.
       */

      case SYS_switch_context:
        {
		sys.Sched_CurrentTask().Task_SetResched();
        }
        break;

      /* R0=SYS_syscall_return:  This a syscall return command:
       *
       *   void up_syscall_return(void);
       *
       * At this point, the following values are saved in context:
       *
       *   R0 = SYS_syscall_return
       *
       * We need to restore the saved return address and return in
       * unprivileged thread mode.
       */

#ifdef CONFIG_LIB_SYSCALL
      case SYS_syscall_return:
        {
          struct tcb_s *rtcb = sched_self();
          int index = (int)rtcb->xcp.nsyscalls - 1;

          /* Make sure that there is a saved syscall return address. */

          DEBUGASSERT(index >= 0);

          /* Setup to return to the saved syscall return address in
           * the original mode.
           */

          regs[REG_PC]         = rtcb->xcp.syscall[index].sysreturn;
          regs[REG_EXC_RETURN] = rtcb->xcp.syscall[index].excreturn;
          rtcb->xcp.nsyscalls  = index;

          /* The return value must be in R0-R1.  dispatch_syscall() temporarily
           * moved the value for R0 into R2.
           */

          regs[REG_R0]         = regs[REG_R2];
        }
        break;
#endif

      /* R0=SYS_task_start:  This a user task start
       *
       *   void up_task_start(main_t taskentry, int argc, FAR char *argv[]) noreturn_function;
       *
       * At this point, the following values are saved in context:
       *
       *   R0 = SYS_task_start
       *   R1 = taskentry
       *   R2 = argc
       *   R3 = argv
       */

#ifdef CONFIG_BUILD_PROTECTED
      case SYS_task_start:
        {
          /* Set up to return to the user-space task start-up function in
           * unprivileged mode.
           */

          regs[REG_PC]         = (uint32_t)USERSPACE->task_startup;
          regs[REG_EXC_RETURN] = EXC_RETURN_UNPRIVTHR;

          /* Change the parameter ordering to match the expectation of struct
           * userpace_s task_startup:
           */

          regs[REG_R0]         = regs[REG_R1]; /* Task entry */
          regs[REG_R1]         = regs[REG_R2]; /* argc */
          regs[REG_R2]         = regs[REG_R3]; /* argv */
        }
        break;
#endif

      /* R0=SYS_pthread_start:  This a user pthread start
       *
       *   void up_pthread_start(pthread_startroutine_t entrypt, pthread_addr_t arg) noreturn_function;
       *
       * At this point, the following values are saved in context:
       *
       *   R0 = SYS_pthread_start
       *   R1 = entrypt
       *   R2 = arg
       */

#if defined(CONFIG_BUILD_PROTECTED) && !defined(CONFIG_DISABLE_PTHREAD)
      case SYS_pthread_start:
        {
          /* Set up to return to the user-space pthread start-up function in
           * unprivileged mode.
           */

          regs[REG_PC]         = (uint32_t)USERSPACE->pthread_startup;
          regs[REG_EXC_RETURN] = EXC_RETURN_UNPRIVTHR;

          /* Change the parameter ordering to match the expectation of struct
           * userpace_s pthread_startup:
           */

          regs[REG_R0]         = regs[REG_R1]; /* pthread entry */
          regs[REG_R1]         = regs[REG_R2]; /* arg */
        }
        break;
#endif

      /* R0=SYS_signal_handler:  This a user signal handler callback
       *
       * void signal_handler(_sa_sigaction_t sighand, int signo,
       *                     FAR siginfo_t *info, FAR void *ucontext);
       *
       * At this point, the following values are saved in context:
       *
       *   R0 = SYS_signal_handler
       *   R1 = sighand
       *   R2 = signo
       *   R3 = info
       *        ucontext (on the stack)
       */

#if defined(CONFIG_BUILD_PROTECTED) && !defined(CONFIG_DISABLE_SIGNALS)
      case SYS_signal_handler:
        {
          struct tcb_s *rtcb   = sched_self();

          /* Remember the caller's return address */

          DEBUGASSERT(rtcb->xcp.sigreturn == 0);
          rtcb->xcp.sigreturn  = regs[REG_PC];

          /* Set up to return to the user-space pthread start-up function in
           * unprivileged mode.
           */

          regs[REG_PC]         = (uint32_t)USERSPACE->signal_handler;
          regs[REG_EXC_RETURN] = EXC_RETURN_UNPRIVTHR;

          /* Change the parameter ordering to match the expectation of struct
           * userpace_s signal_handler.
           */

          regs[REG_R0]         = regs[REG_R1]; /* sighand */
          regs[REG_R1]         = regs[REG_R2]; /* signal */
          regs[REG_R2]         = regs[REG_R3]; /* info */

          /* The last parameter, ucontext, is trickier.  The ucontext
           * parameter will reside at an offset of 4 from the stack pointer.
           */

          regs[REG_R3]         = *(uint32_t*)(regs[REG_SP]+4);
        }
        break;
#endif

      /* R0=SYS_signal_handler_return:  This a user signal handler callback
       *
       *   void signal_handler_return(void);
       *
       * At this point, the following values are saved in context:
       *
       *   R0 = SYS_signal_handler_return
       */

#if defined(CONFIG_BUILD_PROTECTED) && !defined(CONFIG_DISABLE_SIGNALS)
      case SYS_signal_handler_return:
        {
          struct tcb_s *rtcb   = sched_self();

          /* Set up to return to the kernel-mode signal dispatching logic. */

          DEBUGASSERT(rtcb->xcp.sigreturn != 0);

          regs[REG_PC]         = rtcb->xcp.sigreturn;
          regs[REG_EXC_RETURN] = EXC_RETURN_PRIVTHR;
          rtcb->xcp.sigreturn  = 0;
        }
        break;
#endif

      /* This is not an architecture-specific system call.  If NuttX is built
       * as a standalone kernel with a system call interface, then all of the
       * additional system calls must be handled as in the default case.
       */

      default:
        {
#ifdef CONFIG_LIB_SYSCALL
          FAR struct tcb_s *rtcb = sched_self();
          int index = rtcb->xcp.nsyscalls;

          /* Verify that the SYS call number is within range */

          DEBUGASSERT(cmd >= CONFIG_SYS_RESERVED && cmd < SYS_maxsyscall);

          /* Make sure that there is a no saved syscall return address.  We
           * cannot yet handle nested system calls.
           */

          DEBUGASSERT(index < CONFIG_SYS_NNEST);

          /* Setup to return to dispatch_syscall in privileged mode. */

          rtcb->xcp.syscall[index].sysreturn  = regs[REG_PC];
          rtcb->xcp.syscall[index].excreturn  = regs[REG_EXC_RETURN];
          rtcb->xcp.nsyscalls  = index + 1;

          regs[REG_PC]         = (uint32_t)dispatch_syscall;
          regs[REG_EXC_RETURN] = EXC_RETURN_PRIVTHR;

          /* Offset R0 to account for the reserved values */

          regs[REG_R0] -= CONFIG_SYS_RESERVED;
#endif
        }
        break;
    }

  return OK;
}
