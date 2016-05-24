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
last revision: 06/2012
function: hardware abstraction layer
=====================================================================
basic drivers (timer, screen and keyboard)
=====================================================================
*/

#include <prototypes.h>

uint16_t *textmemptr;
int32_t attrib = 0x07;
int32_t csr_x = 0, csr_y = 0;

uint8_t inportb (uint16_t _port){
	uint8_t rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

void outportb (uint16_t _port, uint8_t _data){
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

uint64_t rdtsc(void) {
	uint32_t lo, hi;
	__asm__ __volatile__ (
	"        xorl %%eax,%%eax \n"
	"        cpuid"      // serialize
	::: "%rax", "%rbx", "%rcx", "%rdx");
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	return (uint64_t)hi << 32 | lo;
}

/* Scrolls the screen */
void scroll(void){
	unsigned blank, temp;

	blank = 0x20 | (attrib << 8);

	if(csr_y >= 25){
		temp = csr_y - 25 + 1;
		memcpy (textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);
		memsetw (textmemptr + (25 - temp) * 80, blank, 80);
		csr_y = 25 - 1;
	}
}

/* Updates the hardware cursor */
void move_csr(void){
	unsigned temp;

	temp = csr_y * 80 + csr_x;

	outportb(0x3D4, 14);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, temp);
}

/* Clears the screen */
void cls(){
	unsigned blank;
	int32_t i;

	blank = 0x20 | (attrib << 8);

	for(i = 0; i < 25; i++)
		memsetw (textmemptr + i * 80, blank, 80);

	csr_x = 0;
	csr_y = 0;
	move_csr();
}

/* Puts a character on the screen */
void putch(int8_t c){
	uint16_t *where;
	unsigned att = attrib << 8;

	if(c == 0x08){
		if(csr_x != 0) csr_x--;
	}else if(c == 0x09){
		csr_x = (csr_x + 8) & ~(8 - 1);
	}else if(c == '\r'){
		csr_x = 0;
	}else if(c == '\n'){
		csr_x = 0;
		csr_y++;
	}else if(c >= ' '){
		where = textmemptr + (csr_y * 80 + csr_x);
		*where = c | att;
		csr_x++;
	}

	if(csr_x >= 80){
		csr_x = 0;
		csr_y++;
	}

	scroll();
	move_csr();
}

void settextcolor(uint8_t forecolor, uint8_t backcolor){
	attrib = (backcolor << 4) | (forecolor & 0x0F);
}

/* Sets our text-mode VGA pointer, then clears the screen */
void init_video(void){
	textmemptr = (uint16_t *)0xB8000;
	cls();
}

int32_t timer_ticks = 0;

void timer_phase(int32_t hz){
	int32_t divisor = 1193180 / hz;	   /* Calculate our divisor */
	outportb(0x43, 0x36);			 /* Set our command byte 0x36 */
	outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outportb(0x40, divisor >> 8);	 /* Set high byte of divisor */
}

/* Handles the timer */
void timer_handler(struct regs *r){
	/* Increment our 'tick count' */
	timer_ticks++;
	HF_DispatcherISR((void *)1);
}

void timer_wait(int32_t ticks){
	uint32_t eticks;

	eticks = HF_tick_count + ticks;
	while(HF_tick_count < eticks);
}


/* Sets up the system timer */
void timer_install(){
	/* Installs 'timer_handler' to IRQ0 */
	irq_install_handler(0, timer_handler);
#ifdef DEBUG
	printf("\nKERNEL: timer driver registered");
#endif
}

uint8_t kbdus[256] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
	0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,					/* Right shift */
  '*',
	0,	/* Alt */
  ' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
  '-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
  '+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0,

	0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',		/* Enter key */
	0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,					/* Right shift */
  '*',
	0,	/* Alt */
  ' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
  '-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
  '+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0
};

static uint8_t shift = 0;

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r){
	uint8_t scancode;

	scancode = inportb(0x60);

	if (scancode & 0x80){
		if ((scancode & 0x7f) == 54){
			shift = 0;
		}
	}else{
		if (scancode == 54)
			shift = 1;
		if (shift == 1)
			putch(kbdus[scancode+128]);
		else
			putch(kbdus[scancode]);
	}
}

#define SCROLL_LED 1
#define NUM_LED 2
#define CAPS_LED 4
 
void kbd_update_leds(uint8_t status){
 	uint8_t tmp;
 	while((inportb(0x64)&2)!=0){} //loop until zero
 	outportb(0x60,0xED);
 
 	while((inportb(0x64)&2)!=0){} //loop until zero
 	outportb(0x60,status);
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install(){
	irq_install_handler(1, keyboard_handler);
#ifdef DEBUG
	printf("\nKERNEL: keyboard driver (US layout) registered");
#endif
}

