#include <sys/inc/kernel.h>
#include <arch/armv7/inc/port.h>

uint32_t *up_doirq(int irq, uint32_t *regs)
{
	/*
	Irq::irq_nestedcount++;
	sys.irq.irq_dispatch(irq, regs);
	Irq::irq_nestedcount--;
	if (sys.task.test_task_flag(NEED_RESCHED) && !Irq::irq_nestedcount)
		return sys.task.pick_next_task();
	else
		return regs;
	*/
	return 0;// temp
}
