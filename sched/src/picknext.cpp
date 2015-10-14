#include <sys/inc/kernel.h>
#include <sched/inc/sched.h>

Task &Sched::Sched_PickNext() // critical?
{
	Task &task = LIST_FIRST_ENTRY(sys.task_active);	
	Sched_ResumeScheduler(task);
	DEBUG_PRINT("Sched_PickNext:%ld\n",(uint32_t)&task);
	return task;
}
	

