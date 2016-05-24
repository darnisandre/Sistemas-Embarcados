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
file: mutex.h
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 08/2008
last revision: 02/2010
function: mutex and semaphore implementation
=====================================================================
structures used to build mutexes and semaphores.
=====================================================================
*/

#ifdef HF_OLD_MUTEX
typedef struct{
	uint8_t flag[MAX_TASKS];
	uint8_t last[MAX_TASKS];
} mutex;

#else
typedef struct{
	uint8_t level[MAX_TASKS];
	uint8_t waiting[MAX_TASKS - 1];
} mutex;
#endif

typedef struct{
	uint8_t waiting_queue[MAX_TASKS];
	uint8_t waiting_queue_begin;
	uint8_t waiting_queue_end;
	int32_t count;
} semaphore;

