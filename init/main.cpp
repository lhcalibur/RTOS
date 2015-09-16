#include <sys/inc/sys.h>

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
	sys.init();
	return 0;
}

