#include <new>
#include <sys/inc/sys.h>
#include "stm32f4xx_hal.h"

#ifdef __cplusplus  
extern "C" {  
#endif
	int main(void);
#ifdef __cplusplus  
}
#endif

Sys sys;

int main(void)
{
	new(&sys)Sys();
	while (1);
	return 0;
}

