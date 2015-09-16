#ifndef ARCH_ARMV7_MEMORY_H_
#define ARCH_ARMV7_MEMORY_H_

extern uintptr_t	_ebss;
extern uintptr_t	_estack;
class Memory
{
private:
	uint8_t	*mem_base;
	uint8_t	*mem_end;
public:
	Memory() {}
	void init() { mem_base = (uint8_t *)&_ebss; \
		mem_end = (uint8_t *)&_estack;}
	uint8_t	*GetMembase() const {return mem_base;}
	uint8_t	*GetMemend() const {return mem_end;}
};
#endif /* ARCH_ARMV7_MEMORY_H_ */
