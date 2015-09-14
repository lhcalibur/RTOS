#include <sys/inc/kernel.h>
#include <irq/inc/irq.h>

void Irq::IrqDisable()
{
	sys.port.irqdisable();
}

void Irq::IrqEnable()
{
	sys.port.irqenable();
}

void Irq::IrqSave(irqstate_t &irqstate)
{
	irqstate = sys.port.irqsave();
}

void Irq::IrqRestore(const irqstate_t &irqstate)
{
	sys.port.irqrestore(irqstate);
}

