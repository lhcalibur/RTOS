#ifndef RTOS_MM_H_
#define RTOS_MM_H_
#include <mm/inc/memcore.h>
class MM: private Memcore
{
public:
	MM(): Memcore() {}
	void *malloc(size_t size) {return Memcore_MemcoreAlloc(size);}
	void free(void *) {}
};
#endif /* RTOS_MM_H */
