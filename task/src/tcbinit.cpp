#include <include/types.h>
#include <task/inc/task.h>

int Tcb::init(int priority, size_t stack_size, entry_t entry, uint8_t ttype)
{
	int ret;
	
	createstack(stack_size);
	if (ret < OK) {
		// add code herr
	}

	//List<Tcb &>::INIT_LIST_HEAD(node);

	init_priority = priority;
	sched_priority = priority;

	task_start = Task::taskstart;
	task_entry = entry;

	ttype &= TCB_FLAG_TTYPE_MASK;
	flags &= ~TCB_FLAG_TTYPE_MASK;
	flags |= ttype;

	task_state = TSTATE_TASK_INVALID;

	ret = init_state();
	if (ret < OK) {
		// add code herr
	}

	return OK;
}
