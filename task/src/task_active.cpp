#include <sys/inc/kernel.h>
#include <task/inc/task.h>

int Task::taskactive()
{

	Sched_unblocktask(*this);

	return OK;
}

