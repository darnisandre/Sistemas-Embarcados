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
file: prototypes.h
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 08/2008
last revision: 05/2012
function: definitions and function prototypes
=====================================================================
enough said.
=====================================================================
*/

#include <stdint.h>

/*
constants, tests and transformations
*/
#define NULL			((void *)0)
#define USED			1
#define TRUE			1
#define FALSE			0
#define isprint(c)		(' '<=(c)&&(c)<='~')
#define isspace(c)		((c)==' '||(c)=='\t'||(c)=='\n'||(c)=='\r')
#define isdigit(c)		('0'<=(c)&&(c)<='9')
#define islower(c)		('a'<=(c)&&(c)<='z')
#define isupper(c)		('A'<=(c)&&(c)<='Z')
#define isalpha(c)		(islower(c)||isupper(c))
#define isalnum(c)		(isalpha(c)||isdigit(c))
#define min(a,b)		((a)<(b)?(a):(b))
#define ntohs(A)		(((A)>>8) | (((A)&0xff)<<8))
#define ntohl(A)		(((A)>>24) | (((A)&0xff0000)>>8) | (((A)&0xff00)<<8) | ((A)<<24))

/*
custom Hellfire C library
*/
void putchar(int32_t value);
int32_t kbhit(void);
int32_t getchar(void);
void sleep(uint32_t msec);
void usleep(uint32_t usec);
int8_t *strcpy(int8_t *dst, const int8_t *src);
int8_t *strncpy(int8_t *s1, int8_t *s2, int32_t n);
int8_t *strcat(int8_t *dst, const int8_t *src);
int8_t *strncat(int8_t *s1, int8_t *s2, int32_t n);
int32_t strcmp(const int8_t *s1, const int8_t *s2);
int32_t strncmp(int8_t *s1, int8_t *s2, int32_t n);
int8_t *strstr(const int8_t *string, const int8_t *find);
int32_t strlen(const int8_t *s);
int8_t *strchr(const int8_t *s, int32_t c);
int8_t *strpbrk(int8_t *str, int8_t *set);
int8_t *strsep(int8_t **pp, int8_t *delim);
int8_t *strtok(int8_t *s, const int8_t *delim);
void *memcpy(void *dst, const void *src, uint32_t n);
void *memmove(void *dst, const void *src, uint32_t n);
int32_t memcmp(const void *cs, const void *ct, uint32_t n);
void *memset(void *s, int32_t c, uint32_t n);
uint16_t *memsetw(uint16_t *dest, uint16_t val, int32_t count);
int32_t strtol(const int8_t *s, int8_t **end, int32_t base);
int32_t atoi(const int8_t *s);
int8_t *itoa(int32_t i, int8_t *s, int32_t base);
int32_t puts(const int8_t *str);
int8_t *gets(int8_t *s);
int32_t printf(const int8_t *fmt, ...);
int32_t abs(int32_t n);
int32_t random(void);
void srand(uint32_t seed);

/*
malloc and free
*/
void free(void *ptr);
void *malloc(uint32_t size);
void *calloc(uint32_t qty, uint32_t type_size);
void *realloc(void *ptr, uint32_t size);

