#include <include/types.h>
#include <task/inc/task.h>
#include <sys/inc/kernel.h>

Task::Task(int priority, unsigned int stack_size, entry_t entry, uint8_t ttype): node(*this), Tcb(priority, stack_size, entry, ttype)
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
	
	Sched::Sched_lock();
	sys.task_inactive.list_add(this->node);			
	task_state = TSTATE_TASK_INACTIVE;
	Sched::Sched_unlock();

	// add argv setup

	ret = taskactivate();
	if (ret < OK)
	{
		// add code here;
	}

}
