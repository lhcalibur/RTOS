#ifndef RTOS_IRQ_H_
#define RTOS_IRQ_H_
#include <include/types.h>
class Irq
{
public:
	Irq() {}
	void IrqDisable();
	void IrqEnable();
	void IrqSave(irqstate_t &irqstate);
	void IrqRestore(const irqstate_t &irqstate);
};
#endif /* RTOS_IRQ_H */
