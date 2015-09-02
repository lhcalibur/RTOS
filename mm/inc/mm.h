#ifndef RTOS_MM_H
#define RTOS_MM_H
#include <mm/inc/memcore.h>
class MM: private Memcore
{
public:
	MM(): Memcore() {}
	void *malloc(size_t size) {return MemcoreAlloc(size);}
	void free(void *) {}
};
#endif /* RTOS_MM_H */
