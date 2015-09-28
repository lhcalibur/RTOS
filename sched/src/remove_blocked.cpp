#include <sys/inc/kernel.h>
#include <sched/inc/sched.h>

void Sched::removeblocked(Task &task)
{
	tstate_t task_state = task.Task_State();

	LIST_DEL_INIT((*sys.tlist_table[task_state]), task);

	task.Task_SetState(TSTATE_TASK_INVALID);
}

