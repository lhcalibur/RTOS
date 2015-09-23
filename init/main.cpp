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
	new(sys)Sys();
	BSP_LED_On(LED2);
	return 0;
}

