#include <include/types.h>
#include <task/inc/task.h>
#include <sys/inc/kernel.h>

Task::Task(int priority, unsigned int stack_size, entry_t entry, uint8_t ttype):  Tcb(priority, stack_size, entry, ttype), node(*this)
{
	int ret;
	int errcode;

	/*
	if (ret < OK)
	{
		errcode = -ret;
		goto errout_with_tcb;
	}
	*/
	
	Sys::Sched_lock();
	sys.task_inactive.list_add(this->node);			
	task_state = TSTATE_TASK_INACTIVE;
	DEBUG_PRINT("Task:task_inactive.list_add(this->node) OK\n");
	DEBUG_PRINT("\tthis:%ld\n",(uint32_t)this);
	Sys::Sched_unlock();
	//BSP_LED_On(LED2);

	// add argv setup

	ret = taskactivate();
	if (ret < OK)
	{
		// add code here;
	}

}
