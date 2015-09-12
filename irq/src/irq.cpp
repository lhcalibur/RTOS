#include <sys/inc/kernel.h>
#include <irq/inc/irq.h>

inline void Irq::IrqDisable()
{
	sys.port.irqdisable();
}

inline void Irq::IrqEnable()
{
	sys.port.irqenable();
}

inline void Irq::IrqSave(irqstate_t &irqstate)
{
	irqstate = sys.port.irqsave();
}

void Irq::IrqRestore(const irqstate_t &irqstate)
{
	sys.port.irqrestore(irqstate);
}

