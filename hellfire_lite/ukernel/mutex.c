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
file: mutex.c
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 08/2008
last revision: 08/2011
function: mutex and semaphore implementation
=====================================================================
mutual exclusion implementation (mutex and semaphore).
for mutex, Peterson's algorithm is used.
=====================================================================
*/

#include <prototypes.h>

/*
mutex implementation
*/

#ifdef HF_OLD_MUTEX
void HF_EnterRegion(mutex *m){
	uint8_t i,j,k,l=0;
	uint16_t r;

	i = HF_CurrentTaskId();
	for(j=1;j<=HF_max_index;j++){
		m->flag[i] = j;
		m->last[j] = i;
		for(k=0;k<=HF_max_index;k++){
			if (k == i) continue;
			while(m->flag[k] >= m->flag[i] && m->last[j] == i){
				r = random()%100;
				usleep(r);
			}
		}
	}
}

void HF_LeaveRegion(mutex *m){
	uint8_t i;

	i = HF_CurrentTaskId();
	m->flag[i] = 0;
}

#else
/*
new mutex implementation

(filter algorithm)
*/

void HF_EnterRegion(mutex *m){
	uint8_t i, k, l;

	i = HF_CurrentTaskId();
	for (l = 1; l < MAX_TASKS; ++l){
		m->level[i] = l;
		m->waiting[l] = i;
		for (k = 0; k < MAX_TASKS; k++)
			while(k != i && m->level[k] >= l && m->waiting[l] == i) usleep(1);
	}
}

void HF_LeaveRegion(mutex *m){
	uint8_t i;

	i = HF_CurrentTaskId();
	m->level[i] = 0;
}

#endif
/*
semaphore implementation
*/
void HF_SemInit(semaphore *s, int32_t value){
	uint32_t status;

	status = HF_CriticalBegin();
	s->waiting_queue_begin = 0;
	s->waiting_queue_end = 0;
	s->count = value;
	HF_CriticalEnd(status);
}

void HF_SemWait(semaphore *s){
	uint32_t status;
	uint8_t id;

	status = HF_CriticalBegin();
	s->count--;
	if (s->count < 0){
		id = HF_CurrentTaskId();
		s->waiting_queue[s->waiting_queue_end] = id;
		s->waiting_queue_end = (s->waiting_queue_end + 1) % MAX_TASKS;
		HF_task[id].status = TASK_WAITING;
		HF_CriticalEnd(status);
		HF_TaskYield();
	}else{
		HF_CriticalEnd(status);
		usleep(1);
	}
}

void HF_SemPost(semaphore *s){
	uint32_t status;

	status = HF_CriticalBegin();
	s->count++;
	if (s->count <= 0){
		HF_task[s->waiting_queue[s->waiting_queue_begin]].status = TASK_READY;
		s->waiting_queue_begin = (s->waiting_queue_begin + 1) % MAX_TASKS;
	}
	HF_CriticalEnd(status);
}

