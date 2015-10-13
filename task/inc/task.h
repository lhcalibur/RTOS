#ifndef RTOS_TASK_H
#define RTOS_TASK_H
#include <arch/armv7/inc/port.h>
#include <include/types.h>
#include <include/list.h>
#include <include/config.h>

//typedef int (*entry_t)(int argc, char *argv[]);
// furtherm
typedef int (*entry_t)(void);

/* Values for the Tcb flags bits */

#define TCB_FLAG_TTYPE_SHIFT       (0)      /* Bits 0-1: thread type */
#define TCB_FLAG_TTYPE_MASK        (3 << TCB_FLAG_TTYPE_SHIFT)
#  define TCB_FLAG_TTYPE_TASK      (0 << TCB_FLAG_TTYPE_SHIFT) /* Normal user task */
#  define TCB_FLAG_TTYPE_PTHREAD   (1 << TCB_FLAG_TTYPE_SHIFT) /* User pthread */
#  define TCB_FLAG_TTYPE_KERNEL    (2 << TCB_FLAG_TTYPE_SHIFT) /* Kernel thread */
#define TCB_FLAG_NONCANCELABLE     (1 << 2) /* Bit 2: Pthread is non-cancelable */
#define TCB_FLAG_CANCEL_PENDING    (1 << 3) /* Bit 3: Pthread cancel is pending */
#define TCB_FLAG_POLICY_SHIFT      (4) /* Bit 4-5: Scheduling policy */
#define TCB_FLAG_POLICY_MASK       (3 << TCB_FLAG_POLICY_SHIFT)
#  define TCB_FLAG_SCHED_FIFO      (0 << TCB_FLAG_POLICY_SHIFT) /* FIFO scheding policy */
#  define TCB_FLAG_SCHED_RR        (1 << TCB_FLAG_POLICY_SHIFT) /* Round robin scheding policy */
#  define TCB_FLAG_SCHED_SPORADIC  (2 << TCB_FLAG_POLICY_SHIFT) /* Sporadic scheding policy */
#  define TCB_FLAG_SCHED_OTHER     (3 << TCB_FLAG_POLICY_SHIFT) /* Other scheding policy */
#define TCB_FLAG_EXIT_PROCESSING   (1 << 6) /* Bit 6: Exitting */



enum tstate_e
{
	TSTATE_TASK_INVALID    = 0, /* INVALID      - The TCB is uninitialized */
	TSTATE_TASK_PENDING,        /* READY_TO_RUN - Pending preemption unlock */
	TSTATE_TASK_READYTORUN,     /* READY-TO-RUN - But not running */
	TSTATE_TASK_RUNNING,        /* READY_TO_RUN - And running */

	TSTATE_TASK_INACTIVE,       /* BLOCKED      - Initialized but not yet activated */
	TSTATE_WAIT_SEM,            /* BLOCKED      - Waiting for a semaphore */
#ifndef CONFIG_DISABLE_SIGNALS
	TSTATE_WAIT_SIG,            /* BLOCKED      - Waiting for a signal */
#endif
#ifndef CONFIG_DISABLE_MQUEUE
	TSTATE_WAIT_MQNOTEMPTY,     /* BLOCKED      - Waiting for a MQ to become not empty. */
	TSTATE_WAIT_MQNOTFULL,      /* BLOCKED      - Waiting for a MQ to become not full. */
#endif
	NUM_TASK_STATES             /* Must be last */
};
typedef enum tstate_e tstate_t;

/* The following definitions are determined by tstate_t */

#define FIRST_READY_TO_RUN_STATE TSTATE_TASK_READYTORUN
#define LAST_READY_TO_RUN_STATE  TSTATE_TASK_RUNNING
#define FIRST_BLOCKED_STATE      TSTATE_TASK_INACTIVE
#define LAST_BLOCKED_STATE       (NUM_TASK_STATES-1)



class Tcb
{
	private:
	protected:
		int createstack(size_t stack_size);
		int init_state();
		void savestate();
		void restorestate();
		void copyfullstate(uint32_t *dest, uint32_t *src)
		{
			int i;

			if (src != dest)
			{
				for (i = 0; i < XCPTCONTEXT_REGS; i++)
				{
					*dest++ = *src++;
				}
			}
		}
	public:
		typedef void (*start_t)(void);


		start_t task_start;
		entry_t	task_entry;

		static void taskstart();

		uint8_t sched_priority;
		uint8_t init_priority;

		tstate_t task_state;

		uint16_t flags;

		bool need_resched;

		size_t adj_stack_size;
		void *stack_alloc_ptr;
		void *adj_stack_ptr;
		uint32_t *top_of_stack;

		struct Port::xcptcontext xcp;
		Tcb(int priority, size_t stack_size, entry_t entry, uint8_t ttype);
};

class Task: private Tcb
{
	private:
		friend class Sched;
		int taskcreatetask(int priority, unsigned int stack_size, entry_t entry, uint8_t ttype);
		int taskactivate();
		uint32_t *pregs() {return Tcb::xcp.regs;}
	public:
		Node<Task &> node;
		Task(int priority, unsigned int stack_size, entry_t entry, uint8_t ttype);
		const tstate_t Task_State() const {return Tcb::task_state;}
		void Task_SetState(tstate_t state) {Tcb::task_state = state;}
		inline uint8_t Task_GetSchedPriority() {return Tcb::sched_priority;}
		virtual uint8_t Task_GetTaskType() const = 0;
		inline void Task_SaveTcbState() {Tcb::savestate();}
		inline void Task_RestoreTcbState() {Tcb::restorestate();}
		void Task_SetResched() {need_resched = true;}	// critical?
		void Task_UnsetResched() {need_resched = false;} // critical?
		bool Task_NeedResched() {return need_resched == true;} //critical?
		uint32_t *Task_TopOfStack() {return Tcb::top_of_stack;}
		void Task_SetTopOfStack(uint32_t *ptr) {Tcb::top_of_stack = ptr;}
		entry_t Task_Entry() {return Tcb::task_entry;}
		
};

class UTask: virtual public Task
{
	public:
		UTask(int priority, unsigned int stack_size, entry_t entry): Task(priority, stack_size, entry, TCB_FLAG_TTYPE_TASK) {}
		uint8_t Task_GetTaskType() {return TCB_FLAG_TTYPE_TASK;}
};

#endif /* RTOS_TASK_H */
