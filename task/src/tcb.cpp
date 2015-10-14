#include <include/types.h>
#include <sys/inc/kernel.h>
#include <task/inc/task.h>

Tcb::Tcb(int priority, size_t stack_size, entry_t entry, uint8_t ttype): task_start(taskstart), task_entry(entry), sched_priority(priority), init_priority(priority), task_state(TSTATE_TASK_INVALID), flags(0), need_resched(false),  adj_stack_size(0), stack_alloc_ptr(nullptr), adj_stack_ptr(nullptr)
{
	int ret;
	
	ret = createstack(stack_size);
	if (ret < OK) {
		// add code herr
	}
	DEBUG_PRINT("Tcb:createstack(stack_size) OK\n");
	DEBUG_PRINT("\tstack_size:%ld\n",(uint32_t)stack_size);
	DEBUG_PRINT("\tadj_stack_ptr:%ld\n",(uint32_t)adj_stack_ptr);
	DEBUG_PRINT("\tadj_stack_size:%ld\n",(uint32_t)adj_stack_size);

	ttype &= TCB_FLAG_TTYPE_MASK;
	flags &= ~TCB_FLAG_TTYPE_MASK;
	flags |= ttype;

	ret = init_state();
	if (ret < OK) {
		// add code herr
	}
	DEBUG_PRINT("Tcb:init_state() OK\n");
}
