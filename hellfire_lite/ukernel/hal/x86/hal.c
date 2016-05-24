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
x86 HAL
=====================================================================
*/

#include <prototypes.h>

uint16_t HF_Core(uint16_t core_n){
	return 0;
}

/* basic libc abstraction stuff */
void _putchar(int32_t value){		// polled putchar()
	putch(value);
}

int32_t _kbhit(void){
	return 0;
}

int32_t _getchar(void){			// polled getch()
	return 0;
}

void _sleep(uint64_t msec){
	uint64_t i,j;

	i = rdtsc();
	j = msec * (CPU_SPEED / 1000);
	while(rdtsc() < i + j);
}

void _usleep(uint64_t usec){
	uint64_t i,j;

	i = rdtsc();
	j = usec * (CPU_SPEED / 1000000);
	while(rdtsc() < i + j);
}

/* basic kernel abstraction */

uint32_t HF_ReadCounter(void){
	return 0;
}

uint32_t HF_AsmInterruptEnable(uint32_t state){
	__asm__ __volatile__ ("sti");
	return state;
}

void HF_AsmInterruptDisable(void){
	__asm__ __volatile__ ("cli");
}

int32_t __stack_chk_fail(void){
}

void HF_UpdateCounterMask(void){
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
	HF_AsmInterruptDisable();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	init_video();
}

void HF_AddServices(void){
	HF_AddTask(IdleTask,"Idle Task", 2048, DEFAULT_NICENESS, 1);				// add idle task
}

void HF_HardwareInit(void){
	HF_AsmInterruptDisable();
	keyboard_install();
	timer_phase(TICK_TIME);
	timer_install();
#ifdef DEBUG
	printf("\nKERNEL: hellfire is up and running!\n");
#endif
	HF_schedule = 1;
	HF_AsmInterruptEnable(1);								// enable interrupt
}

void HF_Panic(int32_t cause){
	int8_t buf[20];

	HF_AsmInterruptDisable();
	switch(cause){
		case 0: puts("\nKERNEL: execution aborted - system halted."); break;
		case 1:	puts("\nKERNEL: PANIC!! general protection fault - system halted."); break;
		case 2:	puts("\nKERNEL: PANIC!! stack overflow detected (task ");
			itoa(HF_current_task, buf, 10); 
			puts(buf);
			puts(") - system halted.\n");
			break;
		case 3: puts("\nKERNEL: PANIC!! no more tasks left to dispatch - system halted.\n"); break;
		case 4: puts("\nKERNEL: PANIC!! hardware fault - system halted.");
		case 5: break;
		default:
			puts("\nKERNEL: PANIC!! you should never see this!");
	}
	for(;;);							// stay here forever
}

