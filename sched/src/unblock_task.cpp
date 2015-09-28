#include <sched/inc/sched.h>

void Sched::Sched_unblocktask(Task &task)
{
	int ret;
	irqstate_t irq_flags = Irq_Save();

	removeblocked(task);

	ret = addtoactive(task);

	if (ret == OK) {
		if (Irq_Ininterrupt()) {
			Sched_CurrentTask().Task_SetResched();
		}else {
			Sched_SwitchContext();
		}
	}
	Irq_Restore(irq_flags);
}
