#include <sched/inc/sched.h>

int addtoactive(Task &task)
{
	Task *pos;
	uint8_t task_priority;
	task_priority = task.GetTaskSchedPriority();
	task.TaskStateSet(TSTATE_TASK_READYTORUN);

	if (LIST_EMPTY(task_active)) {
		LIST_ADD(task_active, task);
		goto __need_preemption;
	}else {

		if (LIST_LAST_ENTRY(task_active).GetTaskSchedPriority() > task_priority) {
			LIST_ADD_TAIL(task_active, task);
		}else {	
			LIST_FOR_EACH_ENTRY(task_active, pos) {
				if (pos->GetTaskSchedPriority() < task_priority) {
					LIST_ADD_BEFORE(task_active, task, *pos);
				}
			}
			if (!sched_locked() && IS_LIST_FIRST_ENTRY(task_active, task)) {
				goto __need_preemption;
			}
		}
	}
	return NO;
__need_preemption:
	return OK;
}


