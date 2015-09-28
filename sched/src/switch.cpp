#include <sched/inc/sched.h>

void Sched::Sched_SwitchContext()
{
	svc_switchcontext();
}

void Sched::Sched_Restore(Task &task)
{
	svc_restore_context(task.pregs());
}
