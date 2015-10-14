#include <sched/inc/sched.h>
#include <sys/inc/sys.h>

void Sched::Sched_unblocktask(Task &task)
{
	int ret;
	irqstate_t irq_flags;

	Sys::Irq_Save(irq_flags);
	DEBUG_PRINT("Sys:Irq_Save(irq_flags) OK\n");
	DEBUG_PRINT("\tirq_flags:%d\n",(uint8_t)irq_flags);

	removeblocked(task);
	DEBUG_PRINT("removeblocked(task) OK\n");

	ret = addtoactive(task);
	DEBUG_PRINT("addtoactive(task) OK\n");

	if (ret == OK) {
		DEBUG_PRINT("Need a contextswitch,\n");
		if (Sys::Irq_Ininterrupt()) {
			DEBUG_PRINT("\tIn interrupt\n");
			Sched_CurrentTask().Task_SetResched();
		}else {
			DEBUG_PRINT("\tNot in interrupt\n");
			Sched_SwitchContext();
			DEBUG_PRINT("\tSched_SwitchContext().\n");
		}
	}
	Sys::Irq_Restore(irq_flags);
}
