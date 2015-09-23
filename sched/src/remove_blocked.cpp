#include <sched/inc/sched.h>

void Sched::removeblocked(Task &task)
{
	tstate_t task_state = task.TaskState();

	LIST_DEL_INIT(*sys.sched.tlist_table[task_state], task);

	task.TaskStateSet(TSTATE_TASK_INVALID);
}

