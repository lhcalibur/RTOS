#include <sched/inc/sched.h>
#include <sys/inc/sys.h>

void Sched::Sched_unblocktask(Task &task)
{
	int ret;
	irqstate_t irq_flags;

	Sys::Irq_Save(irq_flags);

	removeblocked(task);

	ret = addtoactive(task);

	if (ret == OK) {
		if (Sys::Irq_Ininterrupt()) {
			Sched_CurrentTask().Task_SetResched();
		}else {
			Sched_SwitchContext();
		}
	}
	Sys::Irq_Restore(irq_flags);
}
