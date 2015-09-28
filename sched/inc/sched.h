#ifndef RTOS_SCHED_H
#define RTOS_SCHED_H
#include <include/list.h>
#include <task/inc/task.h>
#include <include/config.h>

class Sched
{
	private:
		int addtoactive(Task &task);
		void removeblocked(Task &task);
	protected:
		static Task *current_task;
	public:
		//List<Task &> task_pending;
		//List<Task &> task_readytorun;
		//List<Task &> task_running;
		List<Task &> task_active;

		List<Task &> task_inactive;
		List<Task &> task_waitsem;
#ifndef CONFIG_DISABLE_SIGNALS
		List<Task &> task_waitsig;
#endif
#ifndef CONFIG_DISABLE_MQUEUE
		List<Task &> task_waitmqempty;
		List<Task &> task_waitmqfull;
#endif
		List<Task &> *tlist_table[NUM_TASK_STATES] = {
			NULL,
			NULL,//&task_pending,
			&task_active,//&task_readytorun,
			NULL,//&task_running,
			

			&task_inactive,
			&task_waitsem,
#ifndef CONFIG_DISABLE_SIGNALS
			&task_waitsig,
#endif
#ifndef CONFIG_DISABLE_MQUEUE
			&task_waitmqempty,
			&task_waitmqfull
#endif
		};

		Sched (): task_active(), task_inactive(), task_waitsem()
#ifdef CONFIG_DISABLE_SIGNALS
			  ,task_waitsig()
#endif
#ifdef CONFIG_DISABLE_MQUEUE
			  ,task_waitmqempty()
			  ,task_waitmqfull()
#endif
			  {}
		static void Sched_lock() {} // add code here
		static void Sched_unlock() {} // add code here
		static bool Sched_locked() {return 0; } //add code here
		void Sched_unblocktask(Task &);
		Task &Sched_CurrentTask() {return *current_task;}
		static uint32_t *Sched_SetCurrentTask(Task &task);		
		Task &Sched_PickNext();
		void Sched_SwitchContext();
		void Sched_Restore(Task &task);
		void Sched_ResumeScheduler(Task &task) {} //fix
};

#endif /* RTOS_SCHED_H */
