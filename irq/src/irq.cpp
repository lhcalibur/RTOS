#include <sys/inc/kernel.h>
#include <irq/inc/irq.h>

void Irq::Irq_Disable()
{
	Port_Irqdisable();
}

void Irq::Irq_Enable()
{
	Port_Irqenable();
}

void Irq::Irq_Save(irqstate_t &irqstate)
{
	irqstate = Port_Irqsave();
}

void Irq::Irq_Restore(const irqstate_t &irqstate)
{
	Port_Irqrestore(irqstate);
}

