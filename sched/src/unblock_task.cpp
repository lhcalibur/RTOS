#include <sched/inc/sched.h>

void Sched::Sched_unblocktask(Task &task)
{
	int ret;
	irqstate_t irq_flags = Irq_Save();

	removeblocked(task);

	ret = addtoactive(task);
	// add code here
	if (is_ininterrupt()) {

	}else {
		/* Update scheduler parameters */

		sched_resume_scheduler(nexttcb);

		/* Switch context to the context of the task at the head of the
		 * ready to run list.
		 */

		up_switchcontext(rtcb->xcp.regs, nexttcb->xcp.regs);


	}

	Irq_Restore(irq_flags);
}
	

