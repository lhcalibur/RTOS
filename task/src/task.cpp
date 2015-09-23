#include <include/types.h>
#include <task/inc/task.h>
#include <sys/inc/kernel.h>

Task::Task(int priority, unsigned int stack_size, entry_t entry, uint8_t ttype)
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
	// add argv setup

	ret = task_activate();
	if (ret < OK)
	{
		// add code here;
	}

	return OK;
}
