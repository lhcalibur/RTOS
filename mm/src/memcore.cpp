#include <sys/inc/kernel.h>
#include <mm/inc/memcore.h>

void Memcore::init()
{
	Memory::init();
	nextm = (uint8_t *)MEM_ALIGN_NEXT(GetMembase());
	endm = (uint8_t *)MEM_ALIGN_PREV(GetMemend());
}

void *Memcore::memcoreAllocI(size_t size)
{
	void *ptr;

	size = MEM_ALIGN_NEXT(size);
	if (MemcoreStatus() < size) {
		return nullptr;
	}
	ptr = nextm;
	nextm += size;

	return ptr;
}

void *Memcore::MemcoreAlloc(size_t size)
{
	void *ptr;
	irqstate_t irqstate;

	sys.irq.IrqSave(irqstate);
	ptr = memcoreAllocI(size);
	sys.irq.IrqRestore(irqstate);
	return ptr;
}

