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
file: hal.c
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 05/2008
last revision: 05/2012
function: hardware abstraction layer
=====================================================================
hardware dependent functions.
=====================================================================
*/

#include <prototypes.h>

/* basic libc abstraction stuff */
void uart_init(uint32_t baud){
	uint16_t d;

	d = (uint16_t)(CPU_SPEED / baud);
	MemoryWrite(UART_DIVISOR, d);
	MemoryRead(UART_READ);
}

#ifndef DEBUG_PORT
void _putchar(int32_t value){		// polled putchar()
	while((MemoryRead(IRQ_CAUSE) & IRQ_UART_WRITE_AVAILABLE) == 0);
	MemoryWrite(UART_WRITE, value);
}

int32_t _kbhit(void){
	return MemoryRead(IRQ_CAUSE) & IRQ_UART_READ_AVAILABLE;
}

int32_t _getchar(void){			// polled getch()
	while(!kbhit()) ;
	return MemoryRead(UART_READ);
}
#else
void _putchar(int32_t value){		// polled putchar()
	MemoryWrite(DEBUG_ADDR, value);
}

int32_t _kbhit(void){
	return 0;
}

int32_t _getchar(void){			// polled getch()
	return MemoryRead(DEBUG_ADDR);
}
#endif

void _sleep(uint32_t msec){
	volatile uint32_t cur, last, delta, msecs;
	uint32_t cycles_per_msec;

	last = MemoryRead(COUNTER);
	delta = msecs = 0;
	cycles_per_msec = CPU_SPEED / 1000;
	while(msec > msecs){
		cur = MemoryRead(COUNTER);
		if (cur < last)
			delta += (cur + (CPU_SPEED - last));
		else
			delta += (cur - last);
		last = cur;
		if (delta >= cycles_per_msec){
			msecs += delta / cycles_per_msec;
			delta %= cycles_per_msec;
		}
	}
}

void _usleep(uint32_t usec){
	volatile uint32_t cur, last, delta, usecs;
	uint32_t cycles_per_usec;

	last = MemoryRead(COUNTER);
	delta = usecs = 0;
	cycles_per_usec = CPU_SPEED / 1000000;
	while(usec > usecs){
		cur = MemoryRead(COUNTER);
		if (cur < last)
			delta += (cur + (CPU_SPEED - last));
		else
			delta += (cur - last);
		last = cur;
		if (delta >= cycles_per_usec){
			usecs += delta / cycles_per_usec;
			delta %= cycles_per_usec;
		}
	}
}

/* basic kernel abstraction */

uint32_t HF_ReadCounter(void){
	return MemoryRead(COUNTER);
}

void HF_UpdateCounterMask(void){
	uint32_t m;

	m = MemoryRead(IRQ_MASK);					// read interrupt mask
	m ^= (IRQ_COUNTER | IRQ_COUNTER_NOT);				// toggle timer interrupt mask
	MemoryWrite(IRQ_MASK, m);					// write to irq mask register
}

uint32_t HF_SchedulerStatus(void){
	return HF_schedule;
}

void HF_DisableScheduler(){
	HF_schedule = 0;
}

void HF_EnableScheduler(){
	HF_schedule = 1;
}

void HF_BasicHardwareInit(void){
	HF_AsmInterruptEnable(0);
//	uart_init(57600);
}

extern void SystemMonitor(void);

void HF_AddServices(void){
	HF_AddTask(IdleTask,"Idle Task", 1024, DEFAULT_NICENESS, 1);				// add idle task
#ifdef NOC_INTERCONNECT
	HF_AddTask(SystemMonitor, "System Monitor", 2048, DEFAULT_NICENESS, 1);
#endif
}

void HF_TimerInit(void){
	HF_InterruptRegister(IRQ_COUNTER, (HF_funcptr)HF_DispatcherISR);			// register ISRs
	HF_InterruptRegister(IRQ_COUNTER_NOT, (HF_funcptr)HF_DispatcherISR);
	HF_InterruptMaskSet(IRQ_COUNTER);

#ifdef DEBUG
	printf("\nKERNEL: timer driver registered");
#endif
}

void HF_HardwareInit(void){
	HF_InterruptMaskSet(0x0000);							// and clear irq mask

	HF_TimerInit();
#ifdef NOC_INTERCONNECT
	HF_NetworkInterfaceInit();
#endif
//	HF_UartInit();
#ifdef DEBUG
	printf("\nKERNEL: hellfire is up and running!\n");
#endif
	HF_schedule = 1;
	MemoryWrite(IRQ_STATUS, 1);								// enable interrupts
}

void HF_Panic(int32_t cause){
	int8_t buf[20];

	MemoryWrite(IRQ_STATUS, 0);
	switch(cause){
		case 0: puts("\nKERNEL: execution aborted - system halted."); break;
		case 1:	puts("\nKERNEL: PANIC!! general protection fault - system halted."); break;
		case 2:	puts("\nKERNEL: PANIC!! stack overflow detected (task ");
			itoa(HF_current_task, buf, 10); 
			puts(buf);
			puts(") - system halted.\n");
			printf("%x\n", (HF_task[HF_current_task].stack[0] << 24) | (HF_task[HF_current_task].stack[1] << 16) | (HF_task[HF_current_task].stack[2] << 8) | HF_task[HF_current_task].stack[3]);
			break;
		case 3: puts("\nKERNEL: PANIC!! no more tasks left to dispatch - system halted.\n"); break;
		default:
			puts("\nKERNEL: PANIC!! you should never see this!");
	}
	for(;;);							// stay here forever
}

