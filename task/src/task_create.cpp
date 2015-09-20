#include <include/types.h>
#include <task/inc/task.h>
#include <sys/inc/kernel.h>

int Task::TaskCreate(int priority, unsigned int stack_size, entry_t entry)
{
	return createtask(priority, stack_size, entry, TCB_FLAG_TTYPE_TASK);
}


int Task::createtask(int priority, unsigned int stack_size, entry_t entry, uint8_t ttype)
{
	Tcb *tcb;
	int ret;
	int errcode;
	tcb = (Tcb *)sys.malloc(sizeof(Tcb));
	if (!tcb)
	{
		/*
		errcode = ENOMEM;
		goto errout;
		*/
	}
	
	ret = tcb->init(priority, stack_size, entry, ttype);	
	if (ret < OK)
	{
		/*
		errcode = -ret;
		goto errout_with_tcb;
		*/
	}

	//ret = task_activate(tcb);
	if (ret < OK)
	{
		// add code here;
	}

	return OK;
}
