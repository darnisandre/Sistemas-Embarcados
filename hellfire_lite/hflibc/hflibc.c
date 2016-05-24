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
file: hflibc.c
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 05/2008
last revision: 05/2012
function: OS library functions
=====================================================================
implementation of some useful stuff.
=====================================================================
*/

#include <hflibc.h>

void _putchar(int32_t value);
int32_t _kbhit(void);
int32_t _getchar(void);
void _sleep(uint32_t msec);
void _usleep(uint32_t usec);

void putchar(int32_t value){		// polled putchar()
	_putchar(value);
}

int32_t kbhit(void){
	return _kbhit();
}

int32_t getchar(void){			// polled getch()
	return _getchar();
}

void sleep(uint32_t msec){
	_sleep(msec);
}

void usleep(uint32_t usec){
	_usleep(usec);
}

int8_t *strcpy(int8_t *dst, const int8_t *src){
	int8_t *dstSave=dst;
	int32_t c;

	do{
		c = *dst++ = *src++;
	} while(c);
	return dstSave;
}

int8_t *strncpy(int8_t *s1, int8_t *s2, int32_t n){
	int32_t i;
	int8_t *os1;

	os1 = s1;
	for (i = 0; i < n; i++)
		if ((*s1++ = *s2++) == '\0') {
			while (++i < n)
				*s1++ = '\0';
			return(os1);
		}
	return(os1);
}

int8_t *strcat(int8_t *dst, const int8_t *src){
	int32_t c;
	int8_t *dstSave=dst;

	while(*dst)
		++dst;
	do{
		c = *dst++ = *src++;
	} while(c);

	return dstSave;
}

int8_t *strncat(int8_t *s1, int8_t *s2, int32_t n){
	int8_t *os1;

	os1 = s1;
	while (*s1++);
	--s1;
	while ((*s1++ = *s2++))
		if (--n < 0) {
			*--s1 = '\0';
			break;
		}
	return(os1);
}

int32_t strcmp(const int8_t *s1, const int8_t *s2){
	while (*s1 == *s2++)
		if (*s1++ == '\0')
			return(0);

	return(*s1 - *--s2);
}

int32_t strncmp(int8_t *s1, int8_t *s2, int32_t n){
	while (--n >= 0 && *s1 == *s2++)
		if (*s1++ == '\0')
			return(0);

	return(n<0 ? 0 : *s1 - *--s2);
}

int8_t *strstr(const int8_t *string, const int8_t *find){
	int32_t i;
	for(;;){
		for(i = 0; string[i] == find[i] && find[i]; ++i);
		if(find[i] == 0)
			return (char*)string;
		if(*string++ == 0)
			return NULL;
	}
}

int32_t strlen(const int8_t *s){
	int32_t n;

	n = 0;
	while (*s++)
		n++;

	return(n);
}

int8_t *strchr(const int8_t *s, int32_t c){
	while (*s != (int8_t)c) 
		if (!*s++)
			return 0; 

	return (int8_t *)s; 
}

int8_t *strpbrk(int8_t *str, int8_t *set){
	while (*str != '\0'){
		if (strchr(set, *str) == 0)
			++str;
		else
			return (int8_t *) str;
		return 0;
	}
}

int8_t *strsep(int8_t **pp, int8_t *delim){
	int8_t *p, *q;

	if (!(p = *pp))
		return 0;
	if (q = strpbrk (p, delim)){
		*pp = q + 1;
		*q = '\0';
	}else	*pp = 0;

	return p;
}

int8_t *strtok(int8_t *s, const int8_t *delim){
	const int8_t *spanp;
	int32_t c, sc;
	int8_t *tok;
	static int8_t *last;

	if (s == NULL && (s = last) == NULL)
		return (NULL);

	cont:
	c = *s++;
	for (spanp = delim; (sc = *spanp++) != 0;){
		if (c == sc)
		goto cont;
	}

	if (c == 0){
		last = NULL;
		return (NULL);
	}
	tok = s - 1;

	for(;;){
		c = *s++;
		spanp = delim;
		do{
			if ((sc = *spanp++) == c){
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		}while (sc != 0);
	}
}

void *memcpy(void *dst, const void *src, uint32_t n){
	int8_t *r1 = dst;
	const int8_t *r2 = src;

	while (n--)
		*r1++ = *r2++;

	return dst;
}

void *memmove(void *dst, const void *src, uint32_t n){
	int8_t *s = (int8_t *)dst;
	const int8_t *p = (const int8_t *)src;

	if (p >= s){
		while (n--)
			*s++ = *p++;
	}else{
		s += n;
		p += n;
		while (n--)
			*--s = *--p;
	}

	return dst;
}

int32_t memcmp(const void *cs, const void *ct, uint32_t n){
	const uint8_t *r1 = (const uint8_t *)cs;
	const uint8_t *r2 = (const uint8_t *)ct;

	while (n && (*r1 == *r2)) {
		++r1;
		++r2;
		--n;
	}

	return (n == 0) ? 0 : ((*r1 < *r2) ? -1 : 1);
}

void *memset(void *s, int32_t c, uint32_t n){
	uint8_t *p = (uint8_t *)s;

	while (n--)
		*p++ = (uint8_t)c;

	return s;
}

uint16_t *memsetw(uint16_t *dest, uint16_t val, int32_t count){
	uint16_t *temp = (uint16_t *)dest;

	for( ; count != 0; count--) *temp++ = val;
	return dest;
}

int32_t strtol(const int8_t *s, int8_t **end, int32_t base){
	int32_t i;
	uint32_t ch, value=0, neg=0;

	if(s[0] == '-'){
		neg = 1;
		++s;
	}
	if(s[0] == '0' && s[1] == 'x'){
		base = 16;
		s += 2;
	}
	for(i = 0; i <= 8; ++i){
		ch = *s++;
		if('0' <= ch && ch <= '9')
			ch -= '0';
		else if('A' <= ch && ch <= 'Z')
			ch = ch - 'A' + 10;
		else if('a' <= ch && ch <= 'z')
			ch = ch - 'a' + 10;
		else
			break;
		value = value * base + ch;
	}
	if(end)
		*end = (char*)s - 1;
	if(neg)
		value = -(int32_t)value;
	return value;
}

int32_t atoi(const int8_t *s){
	int32_t n, f;

	n = 0;
	f = 0;
	for(;;s++){
		switch(*s){
		case ' ':
		case '\t':
			continue;
		case '-':
			f++;
		case '+':
			s++;
		}
		break;
	}
	while(*s >= '0' && *s <= '9')
		n = n*10 + *s++ - '0';
	return(f?-n:n);
}


int8_t *itoa(int32_t i, int8_t *s, int32_t base){
	int8_t c;
	int8_t *p = s;
	int8_t *q = s;
	uint32_t h;

	if (base == 16){
		h = (uint32_t)i;
		do{
			*q++ = '0' + (h % base);
		} while (h /= base);
		if ((i >= 0) && (i < 16)) *q++ = '0';
		for (*q = 0; p <= --q; p++){
			(*p > '9')?(c = *p + 39):(c = *p);
			(*q > '9')?(*p = *q + 39):(*p = *q);
			*q = c;
		}
	}else{
		if (i >= 0){
			do{
				*q++ = '0' + (i % base);
			} while (i /= base);
		}else{
			*q++ = '-';
			p++;
			do{
				*q++ = '0' - (i % base);
			} while (i /= base);
		}
		for (*q = 0; p <= --q; p++){
			c = *p;
			*p = *q;
			*q = c;
		}
	}
	return s;
}

int32_t puts(const int8_t *str){
	while(*str){
		if(*str == '\n')
			putchar('\r');
		putchar(*str++);
	}
	return 0;
}

int8_t *gets(int8_t *s){
	int32_t c;
	int8_t *cs;

	cs = s;
	while ((c = getchar()) != '\n' && c >= 0)
		*cs++ = c;
	if (c<0 && cs==s)
		return(NULL);
	*cs++ = '\0';
	return(s);
}

/*
printf() and sprintf()
*/
#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_BUF_LEN 20

static void printchar(int8_t **str, int32_t c){	
	if (str){
		**str = c;
		++(*str);
	}else
		putchar((char)c);
}

static int32_t prints(int8_t **out, const int8_t *string, int32_t width, int32_t pad){
	int32_t pc = 0, padchar = ' ';
	int32_t len = 0;
	const int8_t *ptr;

	if (width > 0){
		for (ptr = string; *ptr; ++ptr)
			++len;
		if (len >= width)
			width = 0;
		else
			width -= len;
		if (pad & PAD_ZERO)
			padchar = '0';
	}
	if (!(pad & PAD_RIGHT)){
		for ( ; width > 0; --width){
			printchar(out, padchar);
			++pc;
		}
	}
	for ( ; *string ; ++string){
		printchar(out, *string);
		++pc;
	}
	for ( ; width > 0; --width){
		printchar(out, padchar);
		++pc;
	}

	return pc;
}

static int32_t printi(int8_t **out, int32_t i, int32_t b, int32_t sg, int32_t width, int32_t pad, int32_t letbase){
	int8_t print_buf[PRINT_BUF_LEN];
	int8_t *s;
	int8_t neg = 0, pc = 0;
	uint32_t t, u;

	u = i;
	if (i == 0){
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0){
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';
        
	while (u){
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg){
		if(width && (pad & PAD_ZERO)){
			printchar(out, '-');
			++pc;
			--width;
		}else{
			*--s = '-';
		}
	}

	return pc + prints(out, s, width, pad);
}

static int32_t print(int8_t **out, int32_t *varg){
	int32_t width, pad;
	int32_t pc = 0;
	int8_t *format = (int8_t *)(*varg++);
	int8_t scr[2];

#ifdef FLOATING_POINT
	int32_t i,j;
	int8_t buf[30];
	int32_t f1, precision_n = 6, precision_v = 1;
	float f;
#endif

	for (; *format != 0; ++format){
		if (*format == '%'){
			++format;
			width = pad = 0;
			if (*format == '\0') break;
			if (*format == '%') goto out;
			if (*format == '-'){
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0'){
				++format;
				pad |= PAD_ZERO;
			}
			for ( ; *format >= '0' && *format <= '9'; ++format){
				width *= 10;
				width += *format - '0';
			}
			if(*format == 's'){
				int8_t *s = *((int8_t **)varg++);
				pc += prints(out, s?s:"(null)", width, pad);
				continue;
			}
			if(*format == 'd'){
				pc += printi(out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			if(*format == 'x'){
				pc += printi(out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			if(*format == 'X'){
				pc += printi(out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			if(*format == 'u'){
				pc += printi(out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			if(*format == 'c'){
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints(out, scr, width, pad);
				continue;
			}
#ifdef FLOATING_POINT
			switch(*format){
				case '.':
					// decimal point: 1 to 9 places max. single precision is only about 7 places anyway.
					i = *++format - '0';
					*format++;
					precision_n = i;
					precision_v = 1;
				case 'e':
				case 'E':
				case 'g':
				case 'G':
				case 'f':
					f = (*(float*)&(*varg++));
					if (f < 0.0f){
						putchar('-');
						f = -f;
					}
					itoa((int32_t)f,buf,10);
					j=0;
					while(buf[j]) putchar(buf[j++]);
					putchar('.');
					for(j=0; j<precision_n; j++)
						precision_v *= 10;
					f1 = (f - (int32_t)f) * precision_v;
					i = precision_v / 10;
					while(i > f1){
						putchar('0');
						i /= 10;
					}
					itoa(f1,buf,10);
					j=0;
					if (f1 != 0)
						while(buf[j]) putchar(buf[j++]);
					precision_n = 6;
					precision_v = 1;
					break;
			}
#endif
		}else {
	out:
			printchar(out, *format);
			++pc;
		}
	}
	if (out) **out = '\0';

	return pc;
}

int32_t printf(const int8_t *fmt, ...){
	int32_t *varg = (void *)(&fmt);
	return print(0, varg);
}

int32_t sprintf(int8_t *out, const int8_t *fmt, ...){
	int32_t *varg = (void *)(&fmt);
	return print(&out, varg);
}


int32_t abs(int32_t n){
	return n>=0 ? n:-n;
}

static uint32_t rand1=0xbaadf00d;

int32_t random(void){
	rand1 = rand1 * 1103515245 + 12345;
	return (uint32_t)(rand1 >> 16) & 32767;
}

void srand(uint32_t seed){
	rand1 = seed;
}

/*
software implementation of multiply/divide and 64-bit routines
*/

int32_t __mulsi3(uint32_t a, uint32_t b){
	uint32_t answer = 0;

	while(b){
		if(b & 1)
			answer += a;
		a <<= 1;
		b >>= 1;
	}
	return answer;
}

typedef union{
	int64_t all;
	struct{
#if HF_LITTLE_ENDIAN
		uint32_t low;
		int32_t high;
#else
		int32_t high;
		uint32_t low;
#endif
	} s;
} dwords;

int64_t __muldsi3(uint32_t a, uint32_t b){
	dwords r;

	const int32_t bits_in_word_2 = (int32_t)(sizeof(int32_t) * 8) / 2;
	const uint32_t lower_mask = (uint32_t)~0 >> bits_in_word_2;
	r.s.low = (a & lower_mask) * (b & lower_mask);
	uint32_t t = r.s.low >> bits_in_word_2;
	r.s.low &= lower_mask;
	t += (a >> bits_in_word_2) * (b & lower_mask);
	r.s.low += (t & lower_mask) << bits_in_word_2;
	r.s.high = t >> bits_in_word_2;
	t = r.s.low >> bits_in_word_2;
	r.s.low &= lower_mask;
	t += (b >> bits_in_word_2) * (a & lower_mask);
	r.s.low += (t & lower_mask) << bits_in_word_2;
	r.s.high += t >> bits_in_word_2;
	r.s.high += (a >> bits_in_word_2) * (b >> bits_in_word_2);

	return r.all;
}

int64_t __muldi3(int64_t a, int64_t b){
	dwords x;
	x.all = a;
	dwords y;
	y.all = b;
	dwords r;
	r.all = __muldsi3(x.s.low, y.s.low);
	r.s.high += x.s.high * y.s.low + x.s.low * y.s.high;

	return r.all;
}

static uint32_t DivideMod(uint32_t a, uint32_t b, int32_t doMod){
	uint32_t upper=a, lower=0;
	int32_t i;

	a = b << 31;
	for(i = 0; i < 32; ++i){
		lower = lower << 1;
		if(upper >= a && a && b < 2){
			upper = upper - a;
			lower |= 1;
		}
		a = ((b&2) << 30) | (a >> 1);
		b = b >> 1;
	}
	if(!doMod)
		return lower;
	return upper;
}

uint32_t __udivsi3(uint32_t a, uint32_t b){
	return DivideMod(a, b, 0);
}

uint32_t __umodsi3(uint32_t a, uint32_t b){
	return DivideMod(a, b, 1);
}

int32_t __divsi3(int32_t a, int32_t b){
	int32_t answer, negate=0;

	if(a < 0){
		a = -a;
		negate = !negate;
	}
	if(b < 0){
		b = -b;
		negate = !negate;
	}
	answer = DivideMod(a, b, 0);
	if(negate)
		answer = -answer;
	return answer;
}

int32_t __modsi3(int32_t a, int32_t b){
	int32_t answer, negate=0;

	if(a < 0){
		a = -a;
		negate = !negate;
	}
	if(b < 0){
		b = -b;
		negate = !negate;
	}
	answer = DivideMod(a, b, 1);
	if(negate)
		answer = -answer;
	return answer;
}

int64_t __ashldi3(int64_t u, uint32_t b){
	dwords uu, w;
	uint32_t bm;

	if (b == 0)
		return u;

	uu.all = u;
	bm = 32 - b;

	if (bm <= 0){
		w.s.low = 0;
		w.s.high = (uint32_t) uu.s.low << -bm;
	}else{
		const uint32_t carries = (uint32_t) uu.s.low >> bm;

		w.s.low = (uint32_t) uu.s.low << b;
		w.s.high = ((uint32_t) uu.s.high << b) | carries;
	}
	
	return w.all;
}

int64_t __ashrdi3(int64_t u, uint32_t b){
	dwords uu, w;
	uint32_t bm;

	if (b == 0)
		return u;

	uu.all = u;
	bm = 32 - b;

	if (bm <= 0){
		/* w.s.high = 1..1 or 0..0 */
		w.s.high = uu.s.high >> 31;
		w.s.low = uu.s.low >> -bm;
	}else{
		const uint32_t carries = (uint32_t) uu.s.high << bm;

		w.s.high = uu.s.high >> b;
		w.s.low = ((uint32_t) uu.s.low >> b) | carries;
	}
	
	return w.all;
}

int64_t __lshrdi3(int64_t u, uint32_t b){
	dwords uu, w;
	uint32_t bm;

	if (b == 0)
		return u;

	uu.all = u;
	bm = 32 - b;

	if (bm <= 0){
		w.s.high = 0;
		w.s.low = (uint32_t) uu.s.high >> -bm;
	}else{
		const uint32_t carries = (uint32_t) uu.s.high << bm;
	
		w.s.high = (uint32_t) uu.s.high >> b;
		w.s.low = ((uint32_t) uu.s.low >> b) | carries;
	}
	
	return w.all;
}

uint64_t __udivmoddi4(uint64_t num, uint64_t den, uint64_t *rem_p){
	uint64_t quot = 0, qbit = 1;

	if (den == 0){
		return 1 / ((uint32_t)den);
	}

	while ((int64_t)den >= 0){
		den <<= 1;
		qbit <<= 1;
	}

	while (qbit){
		if (den <= num){
			num -= den;
			quot += qbit;
		}
		den >>= 1;
		qbit >>= 1;
	}

	if (rem_p)
		*rem_p = num;

	return quot;
}

uint64_t __umoddi3(uint64_t num, uint64_t den){
	uint64_t v;

	(void) __udivmoddi4(num, den, &v);
	return v;
}

uint64_t __udivdi3(uint64_t num, uint64_t den){
	return __udivmoddi4(num, den, NULL);
}

int64_t __moddi3(int64_t num, int64_t den){
	int minus = 0;
	int64_t v;

	if (num < 0){
		num = -num;
		minus = 1;
	}
	if (den < 0){
		den = -den;
		minus ^= 1;
	}

	(void) __udivmoddi4(num, den, (uint64_t *)&v);
	if (minus)
		v = -v;

	return v;
}

int64_t __divdi3(int64_t num, int64_t den){
	int minus = 0;
	int64_t v;

	if (num < 0){
		num = -num;
		minus = 1;
	}
	if (den < 0){
		den = -den;
		minus ^= 1;
	}

	v = __udivmoddi4(num, den, NULL);
	if (minus)
		v = -v;

	return v;
}


