#include <sched/inc/sched.h>

int Sched::addtoactive(Task &task)
{
	Task *pos;
	uint8_t task_priority;
	task_priority = task.Task_GetSchedPriority();
	DEBUG_PRINT("addtoactive:task_priority:%d\n",task_priority);
	task.Task_SetState(TSTATE_TASK_READYTORUN);

	if (LIST_EMPTY(task_active)) {			// furtherm
		LIST_ADD(task_active, task);
		DEBUG_PRINT("LIST_EMPTY:LIST_ADD to task_active OK\n");
		Sched_SetCurrentTask(task);
		DEBUG_PRINT("It's the first task.\n");
		return OK;
	}else {

		if (LIST_LAST_ENTRY(task_active).Task_GetSchedPriority() > task_priority) {
			LIST_ADD_TAIL(task_active, task);
		}else {	
			LIST_FOR_EACH_ENTRY(task_active, pos) {
				if (pos->Task_GetSchedPriority() <= task_priority) {
					LIST_ADD_BEFORE(task_active, task, (*pos));
				}
			}
			if (!Sched_locked() && IS_LIST_FIRST_ENTRY(task_active, task)) {
				return OK;
			}
		}
	}
	return NO;
}

