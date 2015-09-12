#ifndef RTOS_SYS_H_
#define RTOS_SYS_H_
#include <arch/armv7/inc/port.h>
#include <irq/inc/irq.h>
#include <mm/inc/mm.h>
class Sys
{
private:
	friend class Memcore;
	friend class Irq;
	Port port;
	Irq irq;
	MM mm;
public:
	Sys() {}
};
#endif /* RTOS_SYS_H */
