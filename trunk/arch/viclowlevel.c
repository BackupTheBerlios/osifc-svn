/*
	file    viclowlevel.c
	author  Werner Oswald
	(c)		Werner Oswald
	mail	osiair@osiworx.com
	date    2009-01-01

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    History:

    2009-07-19 initial version for public release



*/
#include "viclowlevel.h"

#define IRQ_MASK 0x00000080
#define FIQ_MASK 0x00000040

#define ALLINT_MASK (IRQ_MASK | FIQ_MASK)




/*
 * Adjust address at which execution should resume after servicing ISR to compensate for IRQ entry
 * Save the non-banked registers r0-r12 and lr onto the IRQ stack.
 * Get the status of the interrupted program is in SPSR.
 * Push it onto the IRQ stack as well.
 */

// *INDENT-OFF*

void ISR_ENTRY(void)
{
	return;
  asm volatile(						   \
             "sub   lr, lr,#4\n"			   \
             "stmfd sp!,{r0-r12,lr}\n"		           \
             "mrs   r1, spsr\n"			           \
             "stmfd sp!,{r1}"				   \
             );
}

/*
 * Recover SPSR value from stack and restore  its value
 * Pop the return address & the saved general registers from the IRQ stack & return
 */

void ISR_EXIT(void)
{
	return;
  asm volatile(						  \
             "ldmfd sp!,{r1}\n"			          \
             "msr   spsr_c,r1\n"			  \
             "ldmfd sp!,{r0-r12,pc}^"			  \
             );
}










unsigned asm_get_cpsr(void);

unsigned asm_get_cpsr(void)
{
  unsigned long retval;
  asm volatile (" mrs  %0, cpsr" : "=r" (retval) : /* no inputs */  );
  return retval;
}
void asm_set_cpsr(unsigned val);
void asm_set_cpsr(unsigned val)
{
  asm volatile (" msr  cpsr, %0" : /* no outputs */ : "r" (val)  );
}

unsigned enableIRQ(void)
{
  unsigned _cpsr;

  _cpsr = asm_get_cpsr();
  asm_set_cpsr(_cpsr & ~IRQ_MASK);
  return _cpsr;
}
unsigned disableIRQ(void)
{
  unsigned _cpsr;

  _cpsr = asm_get_cpsr();
  asm_set_cpsr(_cpsr | IRQ_MASK);
  return _cpsr;
}
#define FIQ_MASK 0x00000040
unsigned enableFIQ(void)
{
  unsigned _cpsr;

  _cpsr = asm_get_cpsr();
  asm_set_cpsr(_cpsr & ~FIQ_MASK);
  return _cpsr;
}

unsigned disableFIQ(void)
{
  unsigned _cpsr;

  _cpsr = asm_get_cpsr();
  asm_set_cpsr(_cpsr | ~FIQ_MASK);
  return _cpsr;
}


unsigned restoreIRQ(unsigned oldCPSR);
unsigned restoreIRQ(unsigned oldCPSR)
{
  unsigned _cpsr;

  _cpsr = asm_get_cpsr();
  asm_set_cpsr((_cpsr & ~IRQ_MASK) | (oldCPSR & IRQ_MASK));
  return _cpsr;
}
