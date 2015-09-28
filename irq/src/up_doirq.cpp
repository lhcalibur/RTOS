#include <sys/inc/kernel.h>
#include <arch/armv7/inc/port.h>

uint32_t *up_doirq(int irq, uint32_t *regs)
{
	if (!Irq::Irq_Ininterrupt())
		sys.Sched_CurrentTask().Task_SetTopOfStack(regs);	
	Irq::Irq_UpCount();
	Irq::Irq_Dispatch(irq, regs);
	Irq::Irq_DownCount();
	if (!Irq::Irq_Ininterrupt() && !sys.Sched_locked() && sys.Sched_CurrentTask().Task_NeedResched()) { //Sched_locked?
		sys.Sched_CurrentTask().Task_UnsetResched();
		sys.Sched_CurrentTask().Task_SaveTcbState();
		return sys.Sched_SetCurrentTask(sys.Sched_PickNext());
	}else 
		return regs;
}
