#include <task/inc/task.h>

void Tcb::savestate()
{
	uint32_t *dest = xcp.regs;
	uint32_t *src = top_of_stack;

	copyfullstate(dest, src);	
}

void Tcb::restorestate()
{
	uint32_t *dest = top_of_stack;
	uint32_t *src = xcp.regs;

	copyfullstate(dest, src);
}
	
			
