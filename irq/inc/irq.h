#ifndef RTOS_IRQ_H_
#define RTOS_IRQ_H_
#include <include/types.h>
class Irq
{
	protected:
		static uint8_t irq_nestedcount;
	public:
		Irq() {}
		static void Irq_Disable();
		static void Irq_Enable();
		static void Irq_Save(irqstate_t &irqstate);
		static void Irq_Restore(const irqstate_t &irqstate);
		static inline void Irq_UpCount() {irq_nestedcount++;}
		static inline void Irq_DownCount() {irq_nestedcount--;}
		static inline bool Irq_Ininterrupt() {return irq_nestedcount != 0;} //critical ?
		static void Irq_Dispatch(int irq, uint32_t *regs) {}//fix
};
#endif /* RTOS_IRQ_H */
