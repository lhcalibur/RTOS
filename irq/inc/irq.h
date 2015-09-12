#ifndef RTOS_IRQ_H_
#define RTOS_IRQ_H_
class Irq
{
public:
#include <include/types.h>
	Irq() {}
	inline void IrqDisable();
	inline void IrqEnable();
	inline void IrqSave(irqstate_t &irqstate);
	inline void IrqRestore(const irqstate_t &irqstate);
};
#endif /* RTOS_IRQ_H */
