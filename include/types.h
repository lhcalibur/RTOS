#ifndef RTOS_TYPES_H_
#define RTOS_TYPES_H_
#include <arch/armv7/inc/types.h>
/* POSIX-like OS return values: */

#if !defined(__cplusplus)
#  undef  ERROR
#  define ERROR -1
#endif

#undef  OK
#define OK 0

#undef NO
#define NO 1


typedef	unsigned long long  stkalign_t;

typedef unsigned int	    size_t;

#endif /* RTOS_TYPES_H_ */
