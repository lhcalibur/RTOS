#include <include/types.h>
#include <task/inc/task.h>

Tcb::Tcb(int priority, size_t stack_size, entry_t entry, uint8_t ttype)
{
	int ret;
	
	ret = createstack(stack_size);
	if (ret < OK) {
		// add code herr
	}

	ttype &= TCB_FLAG_TTYPE_MASK;
	flags &= ~TCB_FLAG_TTYPE_MASK;
	flags |= ttype;

	ret = init_state();
	if (ret < OK) {
		// add code herr
	}

	sys.sched.sched_lock();
	sys.sched.LIST_ADD(sys.sched.task_inactive, *getthis());			
	task_state = TSTATE_TASK_INACTIVE;
	sys.sched.sched_unlock();
}
