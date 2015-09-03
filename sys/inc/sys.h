#ifndef RTOS_SYS_H_
#define RTOS_SYS_H_
#include <irq/inc/irq.h>
#include <mm/inc/mm.h>
class Sys
{
private:
	friend class Memcore;
	Irq irq;
	MM mm;
public:
	Sys() {}
};
#endif /* RTOS_SYS_H */
