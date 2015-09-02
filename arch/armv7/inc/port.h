#ifndef ARCH_ARM_PORT_H
#define ARCH_ARM_PORT_H

#include <include/stdint.h>
/****************************************************************************
 * Inline functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

/* Get/set the PRIMASK register */

static inline uint8_t getprimask(void)
{
  uint32_t primask;
  __asm__ __volatile__
    (
     "\tmrs  %0, primask\n"
     : "=r" (primask)
     :
     : "memory");

  return (uint8_t)primask;
}

static inline void setprimask(uint32_t primask)
{
  __asm__ __volatile__
    (
      "\tmsr primask, %0\n"
      :
      : "r" (primask)
      : "memory");
}

/* Get/set the BASEPRI register.  The BASEPRI register defines the minimum
 * priority for exception processing. When BASEPRI is set to a nonzero
 * value, it prevents the activation of all exceptions with the same or
 * lower priority level as the BASEPRI value.
 */

static inline uint8_t getbasepri(void)
{
  uint32_t basepri;

  __asm__ __volatile__
    (
     "\tmrs  %0, basepri\n"
     : "=r" (basepri)
     :
     : "memory");

  return (uint8_t)basepri;
}

static inline void setbasepri(uint32_t basepri)
{
  __asm__ __volatile__
    (
      "\tmsr basepri, %0\n"
      :
      : "r" (basepri)
      : "memory");
}


/* Disable IRQs */

static inline void irqdisable(void)
{
  __asm__ __volatile__ ("\tcpsid  i\n");
}

/* Save the current primask state & disable IRQs */

static inline irqstate_t irqsave(void)
{
  unsigned short primask;

  /* Return the current value of primask register and set
   * bit 0 of the primask register to disable interrupts
   */

  __asm__ __volatile__
    (
     "\tmrs    %0, primask\n"
     "\tcpsid  i\n"
     : "=r" (primask)
     :
     : "memory");

  return primask;
}

/* Enable IRQs */

static inline void irqenable(void)
{
  setbasepri(0);
  __asm__ __volatile__ ("\tcpsie  i\n");
}

/* Restore saved primask state */

static inline void irqrestore(irqstate_t flags)
{
  /* If bit 0 of the primask is 0, then we need to restore
   * interrupts.
   */

  __asm__ __volatile__
    (
      "\ttst    %0, #1\n"
      "\tbne.n  1f\n"
      "\tcpsie  i\n"
      "1:\n"
      :
      : "r" (flags)
      : "memory");
}

/* Get/set IPSR */

static inline uint32_t getipsr(void)
{
  uint32_t ipsr;
  __asm__ __volatile__
    (
     "\tmrs  %0, ipsr\n"
     : "=r" (ipsr)
     :
     : "memory");

  return ipsr;
}

static inline void setipsr(uint32_t ipsr)
{
  __asm__ __volatile__
    (
      "\tmsr ipsr, %0\n"
      :
      : "r" (ipsr)
      : "memory");
}

/* Get/set CONTROL */

static inline uint32_t getcontrol(void)
{
  uint32_t control;
  __asm__ __volatile__
    (
     "\tmrs  %0, control\n"
     : "=r" (control)
     :
     : "memory");

  return control;
}

static inline void setcontrol(uint32_t control)
{
  __asm__ __volatile__
    (
      "\tmsr control, %0\n"
      :
      : "r" (control)
      : "memory");
}

#endif /* __ASSEMBLY__ */

#endif /* ARCH_ARM_PORT_H */
