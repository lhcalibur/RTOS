#include <sys/inc/sys.h>
#include <task/inc/task.h>

uint8_t Irq::irq_nestedcount = 0;
Task *Sched::current_task = nullptr;

static int idle_task() {
	BSP_LED_On(LED2);
	while (1);
}

Sys::Sys():Port(), Irq(), MM(), Sched()
{
	DEBUG_PRINT("Hello, we are in Sys::Sys();\n");
	KTask Idle_Process(1, 256, idle_task);
	//BSP_LED_On(LED2);
}
