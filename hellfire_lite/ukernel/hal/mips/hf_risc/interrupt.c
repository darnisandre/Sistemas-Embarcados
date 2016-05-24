/*
    This file is part of HellFire.

    HellFire is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HellFire is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with HellFire.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
=====================================================================
file: interrupt.c
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 05/2008
last revision: 07/2010
function: interrupt control
=====================================================================
enough said
=====================================================================
*/

#include <prototypes.h>

static HF_funcptr HF_isr[16] = {[0 ... 15] = NULL};	// pointers to interrupt service routines

/*
interrupt management routines
*/
uint32_t HF_InterruptStatus(void){			// check interrupt status
	return MemoryRead(IRQ_CAUSE);
}

uint32_t HF_InterruptMaskRead(void){			// check interrupt status
	return MemoryRead(IRQ_MASK);
}

uint32_t HF_InterruptMaskSet(uint32_t mask){		// interrupt vector mask set
	uint32_t m, state;

	state = HF_CriticalBegin();
	m = MemoryRead(IRQ_MASK) | mask;
	MemoryWrite(IRQ_MASK, m);
	HF_CriticalEnd(state);
	return m;
}

uint32_t HF_InterruptMaskClear(uint32_t mask){		// interrupt vector mask clear
	uint32_t m, state;

	state = HF_CriticalBegin();
	m = MemoryRead(IRQ_MASK) & ~mask;
	MemoryWrite(IRQ_MASK, m);
	HF_CriticalEnd(state);
	return m;
}

void HF_InterruptRegister(uint32_t mask, HF_funcptr funcptr){
	int32_t i;

	for(i=0;i<16;++i)
		if(mask & (1<<i))
			HF_isr[i] = funcptr;
}

/*
interrupt service routine

this badass motherfucker is called every time an interrupt occours (if not masked,
of course), and interrupts are already disabled. all it does is to perform a run on
the interrupt service routine pointer array and verify if there is a handler for the
interrupt. if so, it calls it. if not, it clears the mask for that interrupt (which
should be cleared anyway) and then the process iterates. when done, the function
returns and cpu context is restored to the state before the interrupt. an exception
is a timer interrupt, when in this case control is restored by the dispacher, since
the task was preempted and it should really happen that way.
*/
void HF_InterruptServiceRoutine(uint32_t cause, uint32_t *stack){		// the interrupt handler
	int32_t i;
	uint32_t m;

	i=0;
	do{
		if(cause & 1){					// check current bit
			if(HF_isr[i]){				// is there a handler?
				HF_isr[i](stack);		// call it
			}else{
//				HF_InterruptMaskClear(1<<i);	// clear mask for this bit
				m = MemoryRead(IRQ_MASK) & ~(1<<i);
				MemoryWrite(IRQ_MASK, m);		// clear mask for this bit
			}
		}
		cause>>=1;					// set next bit
		++i;
	} while(cause);
	return;
}

