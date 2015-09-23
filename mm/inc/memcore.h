#ifndef RTOS_MEMCORE_H_
#define RTOS_MEMCORE_H_
#include <include/types.h>
#include <arch/armv7/inc/memory.h>

class Memcore: public Memory
{
private:
#define	MEM_ALIGN_SIZE	sizeof(stkalign_t)
#define MEM_ALIGN_MASK	(MEM_ALIGN_SIZE - 1U)
#define MEM_ALIGN_PREV(p)   ((size_t)(p) & ~MEM_ALIGN_MASK)
#define MEM_ALIGN_NEXT(p)   MEM_ALIGN_PREV((size_t)(p) + MEM_ALIGN_MASK)
#define MEM_IS_ALIGNED(p)   (((size_t)(p) & MEM_ALIGN_MASK) == 0U)
	uint8_t	*nextm;
	uint8_t	*endm;
	void *memcoreAllocI(size_t size);
public:
	Memcore(): Memory();
	void *MemcoreAlloc(size_t size);
	size_t MemcoreStatus() const {return (size_t)(endm - nextm);}
};
#endif /* RTOS_MEMCORE_H */
