#include <sys/inc/kernel.h>
#include <task/inc/task.h>
//#include <stdlib.h>

void Tcb::taskstart()
{
	int exitcode;
	DEBUG_PRINT("In Tcb:taskstart.\n");
	entry_t maint = sys.Sched_CurrentTask().Task_Entry();


	// furtherm

	exitcode = maint();

	//exit(exitcode);	
}
