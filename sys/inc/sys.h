#ifndef RTOS_SYS_H
#define RTOS_SYS_H
class Sys
{
private:
	friend class Memcore;
	Irq Irq;
	Memcore Memcore;
public:
	Sys() {}
};
#endif /* RTOS_SYS_H */
