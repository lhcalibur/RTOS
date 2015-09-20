#ifndef RTOS_SYS_H_
#define RTOS_SYS_H_
#include <arch/armv7/inc/port.h>
#include <irq/inc/irq.h>
#include <mm/inc/mm.h>
#include <task/inc/task.h>
#include <sched/inc/sched.h>
class Sys
{
private:
	friend class Memcore;
	friend class Irq;
	Port port;
	Irq irq;
	MM mm;
	Task task; // add init further
	Sched sched;
public:
	Sys() {}
	void init();
	void *malloc(size_t size) {return mm.malloc(size);}
	int TaskCreate(int priority, unsigned int stack_size, entry_t entry) {return task.TaskCreate(priority, stack_size, entry);}

};
#endif /* RTOS_SYS_H */
