#ifndef RTOS_IRQ_H_
#define RTOS_IRQ_H_
#include <arch/armv7/inc/port.h>
class Irq: virtual private Port
{
public:
#include <include/types.h>
	Irq() {}
	inline void IrqDisable() {irqdisable();}
	inline void IrqEnable() {irqenable();}
	void IrqSave(irqstate_t &irqstate) {irqstate = irqsave();}
	void IrqRestore(const irqstate_t &irqstate) {irqrestore(irqstate);}
};
#endif /* RTOS_IRQ_H */
