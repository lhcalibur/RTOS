#ifndef ARCH_ARMV7_MEMORY_H
#define ARCH_ARMV7_MENORY_H
#include <include/stdint.h>
class Memory
{
private:
	extern _uintptr_t	_ebss;
	extern _uintptr_t	_estack;
	uint8_t	*mem_base;
	uint8_t	*mem_end;
public:
	Memory(): mem_base(&_ebss), mem_end(&_estack) {}
	uint8_t	*GetMembase() const {return mem_base;}
	uint8_t	*GetMemend() const {return mem_end;}
};
#endif /* ARCH_ARMV7_MENORY_H */
