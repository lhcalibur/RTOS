#include <sys/inc/kernel.h>
#include <task/inc/task.h>
#include <stdlib.h>

void Tcb::taskstart()
{
	int exitcode;
	entry_t main = sys.Sched_CurrentTask().Task_Entry();


	// furtherm

	exitcode = main();

	//exit(exitcode);	
}
