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
last revision: 08/2011
function: hardware abstraction layer definitions
=====================================================================
definitions that are hardware dependent.
=====================================================================
*/

#include <stdint.h>

/* generic definitions - should be declared! */

#define CPU_ARCH "Intel x86"
#define TICK_TIME_PERIOD (TICK_TIME * 100)

/*********** memory access and critical region access  **/
#define MemoryRead(A)			0
#define MemoryWrite(A,V)		
#define HF_CriticalBegin()		HF_AsmInterruptDisable()
#define HF_CriticalEnd(S)		HF_AsmInterruptEnable(1)

/*********** cpu context *************/
#define CONTEXT_SIZE			20		/* in words */
#define CONTEXT_SP			6
#define CONTEXT_PC			7

/*********** other stuff *********************/
#define TASK_DESCRIPTION_SIZE		21
#define TIMER_ISR_OVERHEAD		100		/* in cycles */

/* X86 hardware dependent definitions */

typedef uint32_t context[CONTEXT_SIZE];

struct regs{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

extern int32_t setjmp(context env);
extern void longjmp(context env, int32_t val);
extern void HF_RestoreExecution(context env, int32_t val, int32_t ctask);
extern uint8_t inportb (uint16_t _port);
extern void outportb (uint16_t _port, uint8_t _data);
extern void init_video(void);
extern void putch(int8_t c);
extern void cls();
extern void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
extern void gdt_install();
extern void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
extern void idt_install();
extern void isrs_install();
extern void irq_install_handler(int32_t irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int32_t irq);
extern void irq_install();
extern void timer_wait(int32_t ticks);
extern void timer_install();
extern void timer_phase(int32_t hz);
extern void keyboard_install();
void IdleTask(void);

