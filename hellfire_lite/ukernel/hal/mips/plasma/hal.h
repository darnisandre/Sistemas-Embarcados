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

#define CPU_ARCH "Plasma CPU"
#define TICK_TIME_PERIOD (1<<TICK_TIME) / (CPU_SPEED / 1000000)

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

/*********** hardware memory addesses ***********/
#define MISC_BASE			0x20000000
#define UART_WRITE			0x20000000
#define UART_READ			0x20000000
#define IRQ_MASK			0x20000010
#define IRQ_STATUS			0x20000020
#define GPIO0_OUT			0x20000030
#define GPIO1_OUT			0x20000040
#define GPIOA_IN			0x20000050
#define COUNTER_REG			0x20000060

#define NOC_READ			0x20000070	/*READ*/
#define NOC_WRITE			0x20000080	/*WRITE*/
#define NOC_STATUS			0x20000090	/*STATUS*/
#define NOC_CTRL			0x200000C0	/*CONTROL*/

#define FREQUENCY_REG			0x200000A0	/* simulator only */
#define TICK_TIME_REG			0x200000B0	/* simulator only */
#define ENERGY_MONITOR			0x200000C0	/* not implemented yet */
#define DISPLAY_REG			0x200000C0	/* on spartan3 starter kit */

#define LEDS_REG			0x200000D0	/* on spartan3 starter kit */
#define OUT_FACILITY			0x200000D0	/* not implemented on hw, but yeah on sim */
#define LOG_FACILITY			0x200000E0	/* simulator only */
#define EXIT_TRAP			0x200000F0	/* simulator only */

/*********** interrupt bits **************/
#define IRQ_UART_READ_AVAILABLE		0x01
#define IRQ_UART_WRITE_AVAILABLE	0x02
#define IRQ_COUNTER18_NOT		0x04
#define IRQ_COUNTER18			0x08
#define IRQ_GPIO30_NOT			0x10
#define IRQ_GPIO31_NOT			0x20
#define IRQ_GPIO30			0x40
#define IRQ_GPIO31			0x80
#define IRQ_NOC_READ			0x100

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
extern void HF_TaskLog(void);
extern int32_t HF_UartPuts(const int8_t *str);
extern void HF_TimerInit(void);
extern void HF_UartInit(void);
void IdleTask(void);

