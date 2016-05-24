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
void _putchar(int32_t value){
#ifdef FAST_STDOUT
	MemoryWrite(OUT_FACILITY, value);
#else
	while((MemoryRead(IRQ_STATUS) & IRQ_UART_WRITE_AVAILABLE) == 0);
	MemoryWrite(UART_WRITE, value);	
#endif
}

int32_t _kbhit(void){
	return MemoryRead(IRQ_STATUS) & IRQ_UART_READ_AVAILABLE;
}

int32_t _getchar(void){
	while(!_kbhit()) ;
	return MemoryRead(UART_READ);
}

void _sleep(uint32_t msec){
	volatile uint32_t cur, last, delta, msecs;
	uint32_t cycles_per_msec;

	last = MemoryRead(COUNTER_REG);
	delta = msecs = 0;
	cycles_per_msec = CPU_SPEED / 1000;
	while(msec > msecs){
		cur = MemoryRead(COUNTER_REG);
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

	last = MemoryRead(COUNTER_REG);
	delta = usecs = 0;
	cycles_per_usec = CPU_SPEED / 1000000;
	while(usec > usecs){
		cur = MemoryRead(COUNTER_REG);
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
	return MemoryRead(COUNTER_REG);
}

void HF_UpdateCounterMask(void){
	uint32_t mask;

	mask = MemoryRead(IRQ_MASK);				// read interrupt mask
	mask ^= (IRQ_COUNTER18 | IRQ_COUNTER18_NOT);		// toggle timer interrupt mask
	MemoryWrite(IRQ_MASK, mask);				// write to irq mask register
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
}

extern void SystemMonitor(void);

void HF_AddServices(void){
	HF_AddTask(IdleTask,"Idle Task", 1024, DEFAULT_NICENESS, 1);				// add idle task
#ifdef NOC_INTERCONNECT
	HF_AddTask(SystemMonitor, "System Monitor", 2048, DEFAULT_NICENESS, 1);
//	HF_AddPeriodicTask(SystemMonitor, 5, 1, 5, "System Monitor", 2048, DEFAULT_NICENESS, 1);
#endif
}

void HF_TimerInit(void){
	HF_InterruptRegister(IRQ_COUNTER18, (HF_funcptr)HF_DispatcherISR);			// register ISRs
	HF_InterruptRegister(IRQ_COUNTER18_NOT, (HF_funcptr)HF_DispatcherISR);
	HF_InterruptMaskSet(IRQ_COUNTER18);
#ifdef DEBUG
	printf("\nKERNEL: timer driver registered");
#endif
}

void HF_HardwareInit(void){
	if((MemoryRead(LOG_FACILITY) != 0xa5a5a5a5)){						// detect if running on simulator
		HF_AsmInterruptInit();								// initialize interrupts
	}else{
		MemoryWrite(FREQUENCY_REG, CPU_SPEED);						// set MPSoC frequency register
		MemoryWrite(TICK_TIME_REG, 1<<TICK_TIME);					// set tick time register
	}
	HF_InterruptMaskClear(0xffffffff);							// and clear irq mask

	HF_TimerInit();
#ifdef NOC_INTERCONNECT
	HF_NetworkInterfaceInit();
#endif
//	HF_UartInit();
#ifdef DEBUG
	printf("\nKERNEL: hellfire is up and running!\n");
#endif
	HF_schedule = 1;
	HF_AsmInterruptEnable(1);								// enable interrupts
}

void HF_Panic(int32_t cause){
	int8_t buf[20];

	HF_InterruptMaskClear(0xffffffff);
	switch(cause){
		case 0: puts("\nKERNEL: execution aborted - system halted."); break;
		case 1:	puts("\nKERNEL: PANIC!! general protection fault - system halted."); break;
		case 2:	puts("\nKERNEL: PANIC!! stack overflow detected (task ");
			itoa(HF_current_task, buf, 10); 
			puts(buf);
			puts(") - system halted.\n");
			break;
		case 3: puts("\nKERNEL: PANIC!! no more tasks left to dispatch - system halted.\n"); break;
		default:
			puts("\nKERNEL: PANIC!! you should never see this!");
	}
	MemoryWrite(EXIT_TRAP,0);					// make the simulator happy
	for(;;);							// stay here forever
}

