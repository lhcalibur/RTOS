#include <sys/inc/sys.h>

Sys::Sys():Port(), Irq(), MM(), Sched()
{
	//Irq::irq_nestedcount = 0;
	//current_task = nullptr;
}
