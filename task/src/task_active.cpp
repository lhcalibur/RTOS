#include <task/inc/task.h>
#include <sys/inc/kernel.h>

int Task::taskactivate()
{

	sys.Sched_unblocktask(*this);

	return OK;
}

