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
file: malloc.c
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 10/2008
last revision: 05/2012
function: implementation of malloc() and free() used on kernel space
and available to user space. HF_AddTask() and HF_KillTask() use this
primitives to allocate/deallocate stack HF_heap_info.
=====================================================================
HF_heap_info management routines
=====================================================================
*/

#include <hflibc.h>
#include <malloc.h>

#ifdef CPU_ID
#include <mutex.h>
extern uint32_t HF_free_mem;
static mutex malloc_mutex;
#endif


static mem_chunk *compact(mem_chunk *p, uint32_t nsize){
	uint32_t bsize, psize;
	mem_chunk *best_fit;

	best_fit = p;
	bsize = 0;

	while(psize = p->size, psize){
		if(psize & USED){
			if(bsize != 0){
				best_fit->size = bsize;
				if(bsize >= nsize) return best_fit;
			}
			bsize = 0;
			best_fit = p = (mem_chunk *)((uint32_t)p + (psize & ~USED));
		}else{
			bsize += psize;
			p = (mem_chunk *)((uint32_t)p + psize);
		}
	}

	if(bsize != 0){
		best_fit->size = bsize;
		if(bsize >= nsize) return best_fit;
	}

	return NULL;
}

void free(void *ptr){
	mem_chunk *p;

#ifdef CPU_ID
	HF_EnterRegion(&malloc_mutex);
#endif
	if(ptr){
		p = (mem_chunk *)((uint32_t)ptr - sizeof(mem_chunk));
		p->size &= ~USED;
	}
#ifdef CPU_ID
	HF_LeaveRegion(&malloc_mutex);
#endif
	HF_free_mem = HF_heap_info.free->size;
}

void *malloc(uint32_t size){
	uint32_t fsize;
	mem_chunk *p;

	if(size == 0) return NULL;

#ifdef CPU_ID
	HF_EnterRegion(&malloc_mutex);
#endif
	size  += 3 + sizeof(mem_chunk);
	size >>= 2;
	size <<= 2;

	if((HF_heap_info.free == 0) || (size > HF_heap_info.free->size)){
		HF_heap_info.free = compact(HF_heap_info.heap, size);
		if(HF_heap_info.free == 0){
#ifdef CPU_ID
			HF_LeaveRegion(&malloc_mutex);
#endif
			return NULL;
		}
	}

	p = HF_heap_info.free;
	fsize = HF_heap_info.free->size;

	if(fsize >= size + sizeof(mem_chunk)){
		HF_heap_info.free = (mem_chunk *)((uint32_t)p + size);
		HF_heap_info.free->size = fsize - size;
	}else{
		HF_heap_info.free = 0;
		size = fsize;
	}

	p->size = size | USED;
#ifdef CPU_ID
	HF_LeaveRegion(&malloc_mutex);
#endif
	HF_free_mem = HF_heap_info.free->size;

	return (void *)((uint32_t)p + sizeof(mem_chunk));
}

void *calloc(uint32_t qty, uint32_t type_size){
	void *buf;
	
	buf = (void *)malloc((qty * type_size));
	if (buf)
		memset(buf, 0, (qty * type_size));

	return (void *)buf;
}

void *realloc(void *ptr, uint32_t size){
	mem_chunk *p;
	void *buf;
	uint32_t min_size;

	if (ptr == NULL)
		return (void *)malloc(size);

	p = (mem_chunk *)((uint32_t)ptr - sizeof(mem_chunk));
	if (p->size == size)	
		return ptr;

	buf = (void *)malloc(size);
	if (buf){
		(size < p->size) ? (min_size=size) : (min_size=p->size);
		memcpy(buf, ptr, min_size);
		free(ptr);
	}

	return (void *)buf;
}

void HF_HeapInit(void *heap, uint32_t len){
	len  += 3;
	len >>= 2;
	len <<= 2;
	HF_heap_info.free = HF_heap_info.heap = (mem_chunk *) heap;
	HF_heap_info.free->size = HF_heap_info.heap->size = len - sizeof(mem_chunk);
	*(uint32_t *)((int8_t *)heap + len - 4) = 0;
	HF_free_mem = HF_heap_info.free->size;
}


