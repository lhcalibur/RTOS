#include <sys/inc/sys.h>
#include <task/inc/task.h>

uint8_t Irq::irq_nestedcount = 0;
Task *Sched::current_task = nullptr;

static int nulltask() {
	while (1);
}

static int idletask() {
	BSP_LED_On(LED2);
	while (1) {
		DEBUG_PRINT("hahaha\n");
	}
}

Sys::Sys():Port(), Irq(), MM(), Sched()
{
	DEBUG_PRINT("Hello, we are in Sys::Sys();\n");
	KTask Null_Process(1, 8, nulltask);
	KTask Idle_Process(0, 256, idletask);
	//BSP_LED_On(LED2);
}
