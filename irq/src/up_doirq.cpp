#include <sys/inc/kernel.h>
#include <arch/armv7/inc/port.h>

extern int svcall(int , void *);

uint32_t *up_doirq(int irq, uint32_t *regs)
{
	if (!Sys::Irq_Ininterrupt())
		sys.Sched_CurrentTask().Task_SetTopOfStack(regs);	
	Sys::Irq_UpCount();
	//Sys::Irq_Dispatch(irq, regs);
	// furtherm
	svcall(irq, regs);
	Sys::Irq_DownCount();
	if (!Sys::Irq_Ininterrupt() && !sys.Sched_locked() && sys.Sched_CurrentTask().Task_NeedResched()) { //Sched_locked?
		sys.Sched_CurrentTask().Task_UnsetResched();
		sys.Sched_CurrentTask().Task_SaveTcbState();
		return sys.Sched_SetCurrentTask(sys.Sched_PickNext());
	}else 
		return regs;
}
