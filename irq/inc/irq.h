#ifndef RTOS_IRQ_H_
#define RTOS_IRQ_H_
#include <include/types.h>
class Irq
{
public:
	static uint8_t irq_nestedcount;
	Irq(): irq_nestedcount(0) {}
	void Irq_Disable();
	void Irq_Enable();
	void Irq_Save(irqstate_t &irqstate);
	void Irq_Restore(const irqstate_t &irqstate);
};
#endif /* RTOS_IRQ_H */
