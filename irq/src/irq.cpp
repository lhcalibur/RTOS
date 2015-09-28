#include <sys/inc/kernel.h>
#include <irq/inc/irq.h>

void Irq::Irq_Disable()
{
	sys.Port_Irqdisable();
}

void Irq::Irq_Enable()
{
	sys.Port_Irqenable();
}

void Irq::Irq_Save(irqstate_t &irqstate)
{
	irqstate = sys.Port_Irqsave();
}

void Irq::Irq_Restore(const irqstate_t &irqstate)
{
	sys.Port_Irqrestore(irqstate);
}

