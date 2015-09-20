#ifndef RTOS_SCHED_H
#define RTOS_SCHED_H
#include <include/list.h>

class Sched
{
	private:
		List<Tcb &> task_readytorun;
	public:
		Sched () {}


};

#endif /* RTOS_SCHED_H */
