#include <include/types.h>
#include <task/inc/task.h>
#include <sys/inc/kernel.h>


#ifndef CONFIG_STACK_ALIGNMENT

/* The symbol  __ARM_EABI__ is defined by GCC if EABI is being used.  If you
 * are not using GCC, make sure that CONFIG_STACK_ALIGNMENT is set correctly!
 */

#  ifdef __ARM_EABI__
#    define CONFIG_STACK_ALIGNMENT 8
#  else
#    define CONFIG_STACK_ALIGNMENT 4
#  endif
#endif

/* Stack alignment macros */

#define STACK_ALIGN_MASK    (CONFIG_STACK_ALIGNMENT-1)
#define STACK_ALIGN_DOWN(a) ((a) & ~STACK_ALIGN_MASK)
#define STACK_ALIGN_UP(a)   (((a) + STACK_ALIGN_MASK) & ~STACK_ALIGN_MASK)


int Tcb::createstack(size_t stack_size)
{
	if (!stack_alloc_ptr) {
		stack_alloc_ptr = (uint32_t *)sys.malloc(stack_size);

		if (!stack_alloc_ptr) {
			// add code here
		}
	}

	if (stack_alloc_ptr) {
		size_t top_of_stack;
		size_t size_of_stack;
		top_of_stack = (uint32_t)stack_alloc_ptr + stack_size - 4;

		/* The ARM stack must be aligned; 4 byte alignment for OABI and
		 * 8-byte alignment for EABI. If necessary top_of_stack must be
		 * rounded down to the next boundary
		 */

		top_of_stack = STACK_ALIGN_DOWN(top_of_stack);

		/* The size of the stack in bytes is then the difference between
		 * the top and the bottom of the stack (+4 because if the top
		 * is the same as the bottom, then the size is one 32-bit element).
		 * The size need not be aligned.
		 */

		size_of_stack = top_of_stack - (uint32_t)stack_alloc_ptr + 4;

		/* Save the adjusted stack values in the struct tcb_s */

		adj_stack_ptr  = (uint32_t*)top_of_stack;
		adj_stack_size = size_of_stack;

		/* If stack debug is enabled, then fill the stack with a
		 * recognizable value that we can use later to test for high
		 * water marks.
		 */

#ifdef CONFIG_STACK_COLORATION
		// further 
		//up_stack_color(tcb->stack_alloc_ptr, tcb->adj_stack_size);
#endif
		return OK;
	}
	return ERROR;
}

/****************************************************************************
 * Name: up_stack_color
 *
 * Description:
 *   Write a well know value into the stack
 *
 ****************************************************************************/

#ifdef CONFIG_STACK_COLORATION
void up_stack_color(FAR void *stackbase, size_t nbytes)
{
  /* Take extra care that we do not write outsize the stack boundaries */

  uint32_t *stkptr = (uint32_t *)(((uintptr_t)stackbase + 3) & ~3);
  uintptr_t stkend = (((uintptr_t)stackbase + nbytes) & ~3);
  size_t    nwords = (stkend - (uintptr_t)stackbase) >> 2;

  /* Set the entire stack to the coloration value */

  while (nwords-- > 0)
    {
      *stkptr++ = STACK_COLOR;
    }
}
#endif

