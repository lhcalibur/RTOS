#include <arch/armv7/inc/port.h>

void exception_common(void)
{
	__asm__ __volatile__ 
		(
		 "\tmrs		r0, ipsr\n"				


		 "\ttst		r14, %0\n" 
		 "\tbeq		1f\n"						
		 "\tmrs		r1, psp\n"				
		 "\tmov     sp, r1\n"				

		 "1:\n"
		 "\tmov		r2, sp\n"					
		 "\tadd		r2, %1\n"		
#ifdef CONFIG_ARMV7M_USEBASEPRI
		 "\tmrs		r3, basepri\n"				
#else
		 "\tmrs		r3, primask\n"				
#endif

#ifdef CONFIG_ARCH_FPU

		"\tvstmdb	sp!, {s16-s31}\n"			

#endif

		"\tstmdb	sp!, {r2-r11,r14}\n"		

#ifndef CONFIG_ARCH_HIPRI_INTERRUPT
		"\tcpsid	i\n"						

#else
		"\tmov		r2, #NVIC_SYSH_DISABLE_PRIORITY\n"
		"\tmsr		basepri, r2\n"				
#endif
		"\tmov		r1, sp\n"

		"\tbl		up_doirq\n"				
		"\tmrs		r1, msp\n"					
		"\tcmp		r0, r1\n"					
		"\tbeq		2f\n"						

		"\tadd		r1, r0, %2\n" 	
		"\tldmia	r1!, {r4-r11}\n"			
#ifdef CONFIG_ARCH_FPU
		"\tvldmia	r1!, {s0-s15}\n"		
		"\tldmia	r1, {r2-r3}\n"				
#endif
		"\tldr		r1, [r0, %3]\n"	
#ifdef CONFIG_ARCH_FPU
		"\tstmdb	r1!, {r2-r3}\n"			
		"\tvstmdb	r1!, {s0-s15}\n"			
#endif
		"\tstmdb	r1!, {r4-r11}\n"			
		"\tldmia	r0!, {r2-r11,r14}\n"		
#ifdef CONFIG_ARCH_FPU
		"\tvldmia	r0, {s16-s31}\n"			
#endif

		"\tb		3f\n"						

		"2:\n"
		"\tldmia	r1!, {r2-r11,r14}\n"		
#ifdef CONFIG_ARCH_FPU
		"\tvldmia  r1!, {s16-s31}\n"			
#endif

		"3:\n"
		"\tmrs		r2, control\n"				
		"\ttst		r14, %0\n" 
		"\tbeq		4f\n"						

		"\torr		r2, r2, #1\n"				
		"\tmsr		psp, r1\n"					
		"\tb		5f\n"
		"4:\n"
		"\tbic		r2, r2, #1\n"				
		"\tmsr		msp, r1\n"					
		"5:\n"
		"\tmsr		control, r2\n"				


#ifdef CONFIG_ARMV7M_USEBASEPRI
		"\tmsr		basepri, r3\n"				
#ifndef CONFIG_ARCH_HIPRI_INTERRUPT
		"\tcpsie	i\n"						
#endif

#else
		"\tmsr		primask, r3\n"				
#endif

		"\tbx		r14\n"						
		:: "i" ( EXC_RETURN_PROCESS_STACK ),  "i" ( HW_XCPT_SIZE ), "i" ( SW_XCPT_SIZE ), "i" ( 4*REG_SP ): "cc"  );
}



