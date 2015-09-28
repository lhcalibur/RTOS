#include <sys/inc/kernel.h>
#include <mm/inc/memcore.h>

Memcore::Memcore(): Memory()
{
	nextm = (uint8_t *)MEM_ALIGN_NEXT(Memory_GetMembase());
	endm = (uint8_t *)MEM_ALIGN_PREV(Memory_GetMemend());
}

void *Memcore::memcoreAllocI(size_t size)
{
	void *ptr;

	size = MEM_ALIGN_NEXT(size);
	if (Memcore_MemcoreStatus() < size) {
		return nullptr;
	}
	ptr = nextm;
	nextm += size;

	return ptr;
}

void *Memcore::Memcore_MemcoreAlloc(size_t size)
{
	void *ptr;
	irqstate_t irqstate;

	Irq::Irq_Save(irqstate);
	ptr = memcoreAllocI(size);
	Irq::Irq_Restore(irqstate);
	return ptr;
}

