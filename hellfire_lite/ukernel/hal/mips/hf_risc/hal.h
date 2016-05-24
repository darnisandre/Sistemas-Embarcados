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
file: hal.h
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 05/2008
last revision: 05/2012
function: hardware abstraction layer definitions
=====================================================================
definitions that are hardware dependent.
=====================================================================
*/

#include <stdint.h>

/* generic definitions - should be declared! */

#define CPU_ARCH "HF-RISC"
#define TICK_TIME_PERIOD (1<<TICK_TIME)*100 / (CPU_SPEED / 10000)

/* HAL peripheral addresses and irq lines */
#define IRQ_VECTOR			0xf0000000
#define IRQ_CAUSE			0xf0000010
#define IRQ_MASK			0xf0000020
#define IRQ_STATUS			0xf0000030
#define IRQ_EPC				0xf0000040
#define COUNTER				0xf0000050
#define COMPARE				0xf0000060
#define COMPARE2			0xf0000070
#define EXTIO_IN			0xf0000080
#define EXTIO_OUT			0xf0000090
#define MMAP_MUL_OPA			0xf00000a0
#define MMAP_MUL_OPB			0xf00000b0
#define MMAP_MUL_RESH			0xf00000c0
#define MMAP_MUL_RESL			0xf00000c4
#define DEBUG_ADDR			0xf00000d0
#define UART_WRITE			0xf00000e0
#define UART_READ			0xf00000e0
#define UART_DIVISOR			0xf00000f0


#define NOC_READ			0xf8000010
#define NOC_WRITE			0xf8000020
#define NOC_STATUS			0xf8000030

#define IRQ_COUNTER			0x0001
#define IRQ_COUNTER_NOT			0x0002
#define IRQ_COUNTER2			0x0004
#define IRQ_COUNTER2_NOT		0x0008
#define IRQ_COMPARE			0x0010
#define IRQ_COMPARE2			0x0020
#define IRQ_UART_READ_AVAILABLE		0x0040
#define IRQ_UART_WRITE_AVAILABLE	0x0080

#define EXT_IRQ0			0x0100
#define EXT_IRQ1			0x0200
#define EXT_IRQ2			0x0400
#define EXT_IRQ3			0x0800
#define EXT_IRQ4			0x1000
#define EXT_IRQ5			0x2000
#define EXT_IRQ6			0x4000
#define EXT_IRQ7			0x8000

/*********** memory access and critical region access  **/
#define MemoryRead(A)			(*(volatile unsigned int*)(A))
#define MemoryWrite(A,V)		*(volatile unsigned int*)(A)=(V)
#define HF_CriticalBegin()		HF_AsmInterruptEnable(0)
#define HF_CriticalEnd(S)		HF_AsmInterruptEnable(S)

/*********** cpu context *************/
#define CONTEXT_SIZE			20		/* in words */
#define CONTEXT_GP			9
#define CONTEXT_SP			10
#define CONTEXT_PC			11

/*********** other stuff *********************/
#define TASK_DESCRIPTION_SIZE		21
#define TIMER_ISR_OVERHEAD		100		/* in cycles */

/* MIPS hardware dependent definitions */

typedef uint32_t context[CONTEXT_SIZE];
// context structure (on MIPS):
// 0..8   general registers
// 9      gp (global pointer)
// 10     sp (stack pointer)
// 11     pc (program counter)

extern int32_t setjmp(context env);
extern void longjmp(context env, int32_t val);
extern void HF_RestoreExecution(context env, int32_t val, int32_t ctask);
extern void HF_AsmInterruptInit(void);
extern uint32_t HF_AsmInterruptEnable(uint32_t state);
extern uint32_t HF_InterruptStatus(void);
extern uint32_t HF_InterruptMaskRead(void);
extern uint32_t HF_InterruptMaskSet(uint32_t mask);
extern uint32_t HF_InterruptMaskClear(uint32_t mask);
extern void HF_InterruptRegister(uint32_t mask, HF_funcptr funcptr);
extern void HF_TimerInit(void);
extern void HF_UartInit(void);
void IdleTask(void);

