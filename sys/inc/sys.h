#ifndef RTOS_SYS_H_
#define RTOS_SYS_H_
#include <arch/armv7/inc/port.h>
#include <irq/inc/irq.h>
#include <mm/inc/mm.h>
#include <include/list.h>
#include <task/inc/task.h>
#include <sched/inc/sched.h>
class Sys: private Port, private Irq, private MM, private Sched
{
private:
	
	friend class Memcore;
	friend class Irq;
	friend class Sched;
	friend class Task;
	friend uint32_t *up_doirq(int, uint32_t *);
	friend int up_svcall(int, void *);
	friend void Tcb::taskstart();
	friend int svcall(int irq, void *context);
public:
	Sys();
	void *malloc(size_t size) {return MM::malloc(size);}
	int TaskCreate();
};
#endif /* RTOS_SYS_H */
