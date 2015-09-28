#include <sched/inc/sched.h>

uint32_t *Sched::Sched_SetCurrentTask(Task &task)
{
	if (current_task && current_task->Task_State() == TSTATE_TASK_RUNNING)
		current_task->Task_SetState(TSTATE_TASK_READYTORUN); 
	task.Task_SetState(TSTATE_TASK_RUNNING); 
	current_task = &task; // critical?
	return current_task->pregs();
}

