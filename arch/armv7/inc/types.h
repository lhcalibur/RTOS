#ifndef __ARCH_ARM_INCLUDE_TYPES_H_
#define __ARCH_ARM_INCLUDE_TYPES_H_
#ifndef __ASSEMBLY__

/* These are the sizes of the standard integer types.  NOTE that these type
 * names have a leading underscore character.  This file will be included
 * (indirectly) by include/stdint.h and typedef'ed to the final name without
 * the underscore character.  This roundabout way of doings things allows
 * the stdint.h to be removed from the include/ directory in the event that
 * the user prefers to use the definitions provided by their toolchain header
 * files
 */

typedef signed char        _int8_t;
typedef unsigned char      _uint8_t;

typedef signed short       _int16_t;
typedef unsigned short     _uint16_t;

typedef signed int         _int32_t;
typedef unsigned int       _uint32_t;

typedef signed long long   _int64_t;
typedef unsigned long long _uint64_t;
#define __INT64_DEFINED

/* A pointer is 4 bytes */

typedef signed int         _intptr_t;
typedef unsigned int       _uintptr_t;

typedef _uint64_t	_stkalign_t;

/* This is the size of the interrupt state save returned by irqsave().  For
 * ARM, a 32 register value is returned, for the thumb2, Cortex-M3, the 16-bit
 * primask register value is returned,
 */

#ifdef __thumb2__
#if defined(CONFIG_ARMV7M_USEBASEPRI)
typedef unsigned char      irqstate_t;
#else
typedef unsigned short     irqstate_t;
#endif
#else /* __thumb2__ */
typedef unsigned int       irqstate_t;
#endif /* __thumb2__ */

#endif /* __ASSEMBLY__ */

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/
#endif /* __ARCH_ARM_INCLUDE_TYPES_H */
