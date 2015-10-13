#include <sys/inc/sys.h>

uint8_t Irq::irq_nestedcount = 0;
Task *Sched::current_task = nullptr;

Sys::Sys():Port(), Irq(), MM(), Sched(), idle_process(0, 128, idle_task)
{
}
