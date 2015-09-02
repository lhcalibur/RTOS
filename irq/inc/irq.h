#ifndef RTOS_IRQ_H
#define RTOS_IRQ_H
class Irq
{
private:
#include <arch/armv7/inc/port.h>
public:
#include <include/stdint.h>
	Irq() {}
	inline void IrqDisable() {irqdisable();}
	inline void IrqEnable() {irqenable();}
	void IrqSave(irqstate_t &irqstate) {irqstate = irqsave();}
	void IrqRestore(const irqstate_t &irqstate) {irqrestore(irqstate);}
};
#endif /* RTOS_IRQ_H */
