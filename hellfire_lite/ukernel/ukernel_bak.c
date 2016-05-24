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
file: ukernel.c
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 05/2008
last revision: 05/2012
function: OS microkernel
=====================================================================
the HellFire preemptive microkernel.
=====================================================================
*/

#include <prototypes.h>
#include <malloc.h>

#ifdef DATA_HEAP
uint8_t HF_heap[HEAP_SIZE] = {[0 ... HEAP_SIZE-1] = 0xaa};	// system heap on .data instead of .bss
#endif

/*
stack overflow check

this tiny routine is used by the dispatcher ISR at every context switch. basically, all it does
is compare the stack guard word to the bottom of stack with its original value. if it differs,
than something really wrong has happened (stack overflow) and the structures themselves are probrably
fucked up. so, there is no point32_t to continue, and since we are already screwed, its time
to say goodbye and hang. remember, we don't have a MMU, so that's the best we can do.
*/
static void HF_CheckTaskStack(void){
	if (HF_task_entry->status != TASK_NOT_RUN){
		if ((HF_task_entry->stack[0] != (uint8_t)((STACK_MAGIC >> 24) & 0xff)) ||
		(HF_task_entry->stack[1] != (uint8_t)((STACK_MAGIC >> 16) & 0xff)) ||
		(HF_task_entry->stack[2] != (uint8_t)((STACK_MAGIC >> 8) & 0xff)) ||
		(HF_task_entry->stack[3] != (uint8_t)((STACK_MAGIC) & 0xff))){
			HF_Panic(PANIC_STACK_OVERFLOW);
		}
	}
}
/*
best effort scheduler

a simple and effective way of scheduling best effort tasks. this is called when there aren't realtime tasks ready
to be scheduled. as spected, round robin scheduling is employed, and there is no priority involved. the idle task has
it's chance to run every once in a while, or when there aren't best effort tasks.
*/
static uint8_t HF_TaskBestEffortReschedule(){
	static uint8_t i=0, j=1;

	for(;j<HF_max_index;j++){							// schedule waiting RT tasks, as they
		HF_task_entry = &HF_task[j];						// have priority over BE tasks
		if ((HF_task_entry->ptask) && (HF_task_entry->period == 0) && (HF_task_entry->capacity == 1)){
			if ((HF_task_entry->status == TASK_READY) || (HF_task_entry->status == TASK_NOT_RUN)){			// ready to run?
				return j;
			}
		}			
	}
	j = 1;
	while(1){									// all waiting RT tasks were scheduled, now
		(i<=HF_max_index)?(i++):(i=0);						// look for a BE tasks
		HF_task_entry = &HF_task[i];						// set pointer to tcb address
		if ((HF_task_entry->ptask) && (HF_task_entry->period == 0)){		// is there a best effort task in this slot?
			if ((HF_task_entry->status == TASK_READY) || (HF_task_entry->status == TASK_NOT_RUN)){			// ready to run?
				if (--HF_task_entry->niceness_counter == 0){							// time to run!
					HF_task_entry->niceness_counter = HF_task_entry->niceness;
					return i;
				}
			}
		}
	}
}

/*
default RT scheduler (Rate Monotonic)
*/
uint8_t HF_TaskReschedule(void){
	uint8_t schedule;
	uint8_t i=0;
	uint16_t j=0xffff;

	schedule = 0;
	for(i=1;i<=HF_max_index;i++){
		HF_task_entry = &HF_task[i];
		if ((HF_task_entry->ptask) && (HF_task_entry->period > 0)){
			if ((HF_task_entry->status == TASK_READY) || (HF_task_entry->status == TASK_NOT_RUN)){
				if ((HF_task_entry->period < j) && (HF_task_entry->capacity_counter > 0)){
					j = HF_task_entry->period;
					schedule = i;
				}
				if (--HF_task_entry->priority == 0){
					HF_task_entry->next_tick_count += HF_task_entry->period;
					HF_task_entry->priority = HF_task_entry->period;
					if (HF_task_entry->capacity_counter > 0)
						HF_task_entry->deadline_misses++;
					HF_task_entry->capacity_counter = HF_task_entry->capacity;
				}
			}
		}
	}
	if (schedule == 0){
		HF_task_entry = &HF_task[0];
		return 0;
	}else{
		HF_task_entry = &HF_task[schedule];
		HF_task_entry->capacity_counter--;
		return schedule;
	}
}
/*
the dispatcher ISR

this interrupt service routine is called every system tick by the interrupt handler. interrupts
are disabled before entering the routine. this ISR toggles the timer mask on the IRQ_MASK register,
saves the preempted task context on its TCB, changes the task status to "ready to run later" (if
task is not stopped), increments the system tick counter and calls the dispatcher. before calling
the dispatcher, the task stack is verified for overflows.
the dispatcher also logs task information (if configured as so and running on the simulator).

In short, the dispatcher is one of the most important kernel mechanisms, responsible for calling
the two scheduling stages (RT and BE), updating the TCB and restoring task context.

You are not expected to understand this.
*/
void HF_DispatcherISR(void *arg){
	static uint32_t time_count = 0;
	int32_t rc, status, k;
	uint32_t original_sp, bytes_sp;

	HF_UpdateCounterMask();
	if (HF_schedule == 0) return;

	HF_context_switch_cycles = HF_ReadCounter();

	HF_last_task = HF_current_task;

	HF_task_entry = &HF_task[HF_current_task];
	rc = setjmp(HF_task_entry->task_context);		// save task context on tcb
	if (rc){
		return;						// returned from longjmp()
	}

	HF_tick_count++;
	HF_task_entry->last_tick_time = (HF_ReadCounter() - time_count);
	time_count = HF_ReadCounter();
#ifdef KERNEL_TASK_LOG
	printf("\nKERNEL: t%d", HF_current_task);
#endif
	if (HF_task_entry->status == TASK_RUNNING)		// is it running or stopped?
		HF_task_entry->status = TASK_READY;		// task is ready to run again
	HF_CheckTaskStack();					// check stack of preempted task

	if ((HF_task_entry->type == TASK_ONESHOT) && (HF_task_entry->capacity_counter == 0))
		HF_KillTask(HF_current_task);
	if (HF_tasks > 0){
		HF_current_task = HF_TaskReschedule();						// call the scheduler
		if (HF_current_task == 0)							// no realtime tasks to be scheduled
			HF_current_task = HF_TaskBestEffortReschedule();			// schedule best effort tasks
		HF_task_entry = &HF_task[HF_current_task];					// FIXME: not all scheduling policies implicitly do this!
		HF_task_entry->status = TASK_RUNNING;						// status: running

		HF_task_entry->ticks++;
		HF_context_switch_cycles = (HF_ReadCounter() - HF_context_switch_cycles) + TIMER_ISR_OVERHEAD;

		HF_RestoreExecution(HF_task_entry->task_context, 1, HF_current_task);				// restore task context
		HF_Panic(PANIC_UNKNOWN);
	}else{											// hope we never get here..
		HF_Panic(PANIC_NO_TASKS_LEFT);
	}
}

/*
task yield

this routine is used when a task wants to give up execution, so it tells the kernel that
another task can take control. it differs from a timer preemption because nobody messes
with the counter register here. otherwise, the process is the same. interrupts are
disabled (so we don't get fucked up) and context is saved on the tcb. task status is
set to ready to run again in the future (if not blocked), and the dispatcher is called.
*/
void HF_TaskYield(void){					// lets another task to run
	static uint32_t time_count = 0;
	int32_t rc, status;

	status = HF_CriticalBegin();
	HF_context_switch_cycles = HF_ReadCounter();

	HF_task_entry = &HF_task[HF_current_task];
	rc = setjmp(HF_task_entry->task_context);		// save task context on tcb
	if (rc){
		HF_CriticalEnd(status);
		return;						// returned from longjmp()
	}

	HF_task_entry->last_tick_time = 0;

	if (HF_task_entry->status == TASK_RUNNING)		// is it running or stopped?
		HF_task_entry->status = TASK_READY;		// task is ready to run again
	HF_CheckTaskStack();					// check stack of preempted task

	HF_current_task = HF_TaskBestEffortReschedule();
	HF_task_entry = &HF_task[HF_current_task];		// FIXME: not all scheduling policies implicitly do this!

	HF_task_entry->status = TASK_RUNNING;						// status: running

	HF_task_entry->ticks++;
	HF_context_switch_cycles = (HF_ReadCounter() - HF_context_switch_cycles) + TIMER_ISR_OVERHEAD;

	HF_RestoreExecution(HF_task_entry->task_context, 1, HF_current_task);
	HF_Panic(PANIC_UNKNOWN);
}


uint32_t HF_TaskMemoryUsage(uint8_t id){
	return (uint32_t)HF_task[id].stack + (HF_task[id].stack_size - 4) - HF_task[id].task_context[CONTEXT_SP];
}

uint32_t HF_TaskTicks(uint8_t id){
	return HF_task[id].ticks;
}

uint32_t HF_LastTickTime(uint8_t id){
	return HF_task[id].last_tick_time;
}

uint32_t HF_LastContextSwitchTime(void){
	return HF_context_switch_cycles;
}

int32_t HF_TaskId(int8_t *description){
	int32_t i;

	for (i = 0; i <= HF_max_index; i++){
		if (HF_task[i].ptask){
			if (strcmp(HF_task[i].description, description) == 0)
				return HF_task[i].id;
		}
	}
	return HF_INVALID_NAME;
}

uint8_t HF_CurrentTaskId(void){
	return HF_current_task;
}

uint8_t HF_CurrentCpuId(void){
	return CPU_ID;
}

uint32_t HF_CurrentCpuFrequency(void){
	return (CPU_SPEED/1000000);
}

uint16_t HF_NCores(void) {
#ifdef N_CORES
	return N_CORES;
#else
	return 1;
#endif
}

uint8_t HF_NTasks(void){					// returns number of tasks
	return HF_tasks;
}
int32_t HF_BlockTask(uint8_t id){				// block task from being scheduled
	uint32_t status;

	status = HF_CriticalBegin();
	if (id == 0){
#ifdef DEBUG
		printf("\nKERNEL: can't block the idle task");
#endif
		HF_CriticalEnd(status);
		return HF_INVALID_ID;
	}
	HF_task_entry = &HF_task[id];
	if ((HF_task_entry->ptask == 0) || (id >= MAX_TASKS)){
#ifdef DEBUG
		printf("\nKERNEL: task doesn't exist");
#endif
		HF_CriticalEnd(status);
		return HF_INVALID_ID;
	}
	if (HF_task_entry->status == TASK_NOT_RUN){
#ifdef DEBUG
		printf("\nKERNEL: can't block a task that has not run yet");
#endif
		HF_CriticalEnd(status);
		return HF_INVALID_STATE;
	}
	HF_task_entry->status = TASK_BLOCKED;
#ifdef DEBUG
//	printf("\nKERNEL: task blocked, id: %d", id);
#endif
	HF_CriticalEnd(status);
	return HF_OK;
}

int32_t HF_ResumeTask(uint8_t id){				// resume task scheduling
	uint32_t status;

	status = HF_CriticalBegin();
	if (id == 0){
#ifdef DEBUG
		printf("\nKERNEL: can't resume the idle task");
#endif
		HF_CriticalEnd(status);
		return HF_INVALID_ID;
	}
	HF_task_entry = &HF_task[id];
	if ((HF_task_entry->ptask == 0) || (id >= MAX_TASKS)){
#ifdef DEBUG
		printf("\nKERNEL: task doesn't exist");
#endif
		HF_CriticalEnd(status);
		return HF_INVALID_ID;
	}
	if (HF_task_entry->status == TASK_NOT_RUN){
#ifdef DEBUG
		printf("\nKERNEL: can't resume a task that has not run yet");
#endif
		HF_CriticalEnd(status);
		return HF_INVALID_STATE;
	}

	HF_task_entry->status = TASK_READY;
#ifdef DEBUG
//	printf("\nKERNEL: task resumed, id: %d", id);
#endif
	HF_CriticalEnd(status);
	return HF_OK;
}

int32_t HF_KillTask(uint8_t id){				// kill a task
	uint32_t status;
	uint8_t i;

	status = HF_CriticalBegin();
	if (id == 0){
#ifdef DEBUG
		printf("\nKERNEL: can't kill the idle task");
#endif
		HF_CriticalEnd(status);
		return HF_INVALID_ID;
	}
	HF_task_entry = &HF_task[id];
	if ((HF_task_entry->ptask == 0) || (id >= MAX_TASKS)){
#ifdef DEBUG
		printf("\nKERNEL: task doesn't exist");
#endif
		HF_CriticalEnd(status);
		return HF_INVALID_ID;
	}
	HF_task_entry->ptask = 0;
	free(HF_task_entry->stack);				// free task stack area from the heap
#ifdef NOC_INTERCONNECT
	free(HF_task_entry->packet_in);			// free task packet in buffer
	free(HF_task_entry->packet_out);			// free task packet out buffer
#endif
	HF_task_entry->status = TASK_NOT_RUN;			// it's dead, and will not run anymore
	HF_tasks--;						// one less task in the system
	if (HF_task_entry->period == 0)
		HF_be--;					// less one best effort task
	if (id == HF_max_index){				// optimize if needed, so we'll speed up scheduling
		for(i=0;i<MAX_TASKS;i++){
			if (HF_task[i].ptask != 0)
				HF_max_index = i;
		}
	}

	if (HF_CurrentTaskId() == id){				// task killed itself
#ifdef DEBUG
		printf("\nKERNEL: task commited suicide, id: %d, tasks left: %d",id, HF_tasks);
#endif
		HF_CriticalEnd(status);
		HF_TaskYield();					// so, we don't have to return, just reschedule
	}else{							// someone else killed it
#ifdef DEBUG
		printf("\nKERNEL: task died, id: %d, tasks left: %d",id, HF_tasks);
#endif
		HF_CriticalEnd(status);
	}

	return HF_OK;
}

int32_t HF_AddTask(void (*task)(), int8_t description[], uint32_t stack_size, uint8_t niceness, uint8_t type){
	uint32_t status;
	uint8_t i=0;
	uint32_t j;


	status = HF_CriticalBegin();
	while ((HF_task[i].ptask != 0) && (i < MAX_TASKS))		// check for an empty slot
		i++;

	if (i == MAX_TASKS){						// none free. give up
#ifdef DEBUG
		printf("\nKERNEL: task not added - MAX_TASKS: %d", MAX_TASKS);
#endif
		HF_CriticalEnd(status);
		return HF_EXCEED_MAX_NUM;
	}
	if (i > HF_max_index)						// we are at the end of the list
		HF_max_index = i;

	HF_be++;
	HF_tasks++;							// update task count
	HF_task_entry = &HF_task[i];					// set task pointer to it's entry and fill the task control block
	HF_task_entry->id = i;
	HF_task_entry->type = type;
	for(j=0;((description[j] != '\0') && (j<(TASK_DESCRIPTION_SIZE-1)));j++)
		HF_task_entry->description[j] = description[j];
	for(;j<TASK_DESCRIPTION_SIZE;j++) HF_task_entry->description[j] = '\0';
	HF_task_entry->status = TASK_NOT_RUN;
	HF_task_entry->ticks = 0;
	HF_task_entry->priority = 0;
	HF_task_entry->period = 0;
	HF_task_entry->capacity = 0;
	HF_task_entry->capacity_counter = 0;
	HF_task_entry->deadline = 0;
	HF_task_entry->deadline_counter = 0;
	HF_task_entry->deadline_misses = 0;
	if (niceness < 10) niceness = DEFAULT_NICENESS;
	HF_task_entry->niceness = niceness;
	HF_task_entry->niceness_counter = niceness;
	stack_size += 3;
	stack_size >>= 2;
	stack_size <<= 2;
	HF_task_entry->stack = (uint8_t *)malloc(stack_size);			// allocate stack from the heap
	HF_task_entry->stack_ptr = (void *)HF_task_entry->stack + (stack_size - 4);
	HF_task_entry->ptask = task;						// set task entry point
	HF_task_entry->stack_size = stack_size;
//	setjmp(HF_task_entry->task_context);
	HF_task_entry->task_context[CONTEXT_SP] = (uint32_t)HF_task_entry->stack_ptr;
	HF_task_entry->task_context[CONTEXT_PC] = (uint32_t)HF_task_entry->ptask;
	HF_task_entry->next_tick_count = HF_tick_count + HF_task_entry->period;
	HF_SemInit(&HF_task_entry->ipc_message_wait_send, 0);
	HF_SemInit(&HF_task_entry->ipc_message_wait_recv, 0);
#ifdef NOC_INTERCONNECT
	HF_task_entry->in_packet = 0;
	HF_task_entry->out_packet = 0;
	HF_task_entry->packet_in = (packet_in_buffer_type *)malloc(sizeof(packet_in_buffer_type));
	HF_task_entry->packet_out = (packet_out_buffer_type *)malloc(sizeof(packet_out_buffer_type));
	HF_task_entry->packet_in->start = 0;
	HF_task_entry->packet_in->end = 0;
	HF_task_entry->packets_sent = 0;
	HF_task_entry->packets_received = 0;
	HF_task_entry->packets_lost = 0;
	HF_task_entry->packets_seqerror = 0;
#endif

#ifdef NOC_INTERCONNECT
	if ((HF_task_entry->stack) && (HF_task_entry->packet_in) && (HF_task_entry->packet_out)){
#else
	if (HF_task_entry->stack){
#endif
		HF_task_entry->stack[0] = (uint8_t)((STACK_MAGIC >> 24) & 0xff);
		HF_task_entry->stack[1] = (uint8_t)((STACK_MAGIC >> 16) & 0xff);
		HF_task_entry->stack[2] = (uint8_t)((STACK_MAGIC >> 8) & 0xff);
		HF_task_entry->stack[3] = (uint8_t)((STACK_MAGIC) & 0xff);
#ifdef DEBUG
		printf("\nKERNEL: [%s] (BE), id: %d, niceness: %d, addr: %x, sp: %x, ss: %d bytes", HF_task_entry->description, HF_task_entry->id, HF_task_entry->niceness, HF_task_entry->ptask, HF_task_entry->stack_ptr, stack_size);
#endif
	}else{										// we ran out of memory =(
		HF_task_entry->ptask = 0;
		HF_tasks--;
#ifdef NOC_INTERCONNECT
		if (HF_task_entry->packet_in)
			free(HF_task_entry->packet_in);
		if (HF_task_entry->packet_out)
			free(HF_task_entry->packet_out);
#endif
#ifdef DEBUG
		printf("\nKERNEL: task not added (out of memory)");
#endif
		i = HF_OUT_OF_MEMORY;
	}
	HF_CriticalEnd(status);

	return i;
}

/*
HF_AddPeriodicTask()

this system call adds realtime tasks to the system. parameters are period, capacity and deadline among others.
*/
int32_t HF_AddPeriodicTask(void (*task)(), uint16_t period, uint16_t capacity, uint16_t deadline, int8_t description[], uint32_t stack_size, uint8_t niceness, uint8_t type){
	uint32_t status;
	uint8_t i=0;
	uint32_t j;

	if ((period < capacity) || (capacity == 0))			// someone was trying to add an impossible task
		return HF_INVALID_PARAMETER;

	status = HF_CriticalBegin();
	while ((HF_task[i].ptask != 0) && (i < MAX_TASKS))		// check for an empty slot
		i++;

	if (i == MAX_TASKS){						// none free. give up
#ifdef DEBUG
		printf("\nKERNEL: task not added - MAX_TASKS: %d", MAX_TASKS);
#endif
		HF_CriticalEnd(status);
		return HF_OUT_OF_MEMORY;
	}
	if (i > HF_max_index)						// we are at the end of the list
		HF_max_index = i;

	HF_tasks++;							// update task count
	HF_task_entry = &HF_task[i];					// set task pointer to it's entry and fill the task control block
	HF_task_entry->id = i;
	HF_task_entry->type = type;
	for(j=0;((description[j] != '\0') && (j<(TASK_DESCRIPTION_SIZE-1)));j++)
		HF_task_entry->description[j] = description[j];
	for(;j<TASK_DESCRIPTION_SIZE;j++) HF_task_entry->description[j] = '\0';
	HF_task_entry->status = TASK_NOT_RUN;
	HF_task_entry->ticks = 0;
	HF_task_entry->priority = period;
	HF_task_entry->period = period;
	HF_task_entry->capacity = capacity;

	HF_task_entry->capacity_counter = capacity;
	HF_task_entry->deadline = deadline;
	HF_task_entry->deadline_counter = 0;
	HF_task_entry->deadline_misses = 0;
	if (niceness < 10) niceness = DEFAULT_NICENESS;
	HF_task_entry->niceness = niceness;
	HF_task_entry->niceness_counter = niceness;
	stack_size += 3;
	stack_size >>= 2;
	stack_size <<= 2;
	HF_task_entry->stack = (uint8_t *)malloc(stack_size);		// allocate stack from the heap
	HF_task_entry->stack_ptr = (void *)HF_task_entry->stack + (stack_size - 4);
	HF_task_entry->ptask = task;					// set task entry point
	HF_task_entry->stack_size = stack_size;
//	setjmp(HF_task_entry->task_context);
	HF_task_entry->task_context[CONTEXT_SP] = (uint32_t)HF_task_entry->stack_ptr;
	HF_task_entry->task_context[CONTEXT_PC] = (uint32_t)HF_task_entry->ptask;
	HF_task_entry->next_tick_count = HF_tick_count + HF_task_entry->period;
	HF_SemInit(&HF_task_entry->ipc_message_wait_send, 0);
	HF_SemInit(&HF_task_entry->ipc_message_wait_recv, 0);
#ifdef NOC_INTERCONNECT
	HF_task_entry->in_packet = 0;
	HF_task_entry->out_packet = 0;
	HF_task_entry->packet_in = (packet_in_buffer_type *)malloc(sizeof(packet_in_buffer_type));
	HF_task_entry->packet_out = (packet_out_buffer_type *)malloc(sizeof(packet_out_buffer_type));
	HF_task_entry->packet_in->start = 0;
	HF_task_entry->packet_in->end = 0;
	HF_task_entry->packets_sent = 0;
	HF_task_entry->packets_received = 0;
	HF_task_entry->packets_lost = 0;
	HF_task_entry->packets_seqerror = 0;
#endif

#ifdef NOC_INTERCONNECT
	if ((HF_task_entry->stack) && (HF_task_entry->packet_in) && (HF_task_entry->packet_out)){
#else
	if (HF_task_entry->stack){
#endif
		HF_task_entry->stack[0] = (uint8_t)((STACK_MAGIC >> 24) & 0xff);
		HF_task_entry->stack[1] = (uint8_t)((STACK_MAGIC >> 16) & 0xff);
		HF_task_entry->stack[2] = (uint8_t)((STACK_MAGIC >> 8) & 0xff);
		HF_task_entry->stack[3] = (uint8_t)((STACK_MAGIC) & 0xff);
#ifdef DEBUG
		printf("\nKERNEL: [%s] (RT), id: %d, p:%d, c:%d, d:%d, addr: %x, sp: %x, ss: %d bytes", HF_task_entry->description, HF_task_entry->id, HF_task_entry->period, HF_task_entry->capacity, HF_task_entry->deadline, HF_task_entry->ptask, HF_task_entry->stack_ptr, stack_size);
#endif
	}else{										// we ran out of memory =(
		HF_task_entry->ptask = 0;
		HF_tasks--;
#ifdef NOC_INTERCONNECT
		if (HF_task_entry->packet_in)
			free(HF_task_entry->packet_in);
		if (HF_task_entry->packet_out)
			free(HF_task_entry->packet_out);
#endif
#ifdef DEBUG
		printf("\nKERNEL: task not added (out of memory)");
#endif
		i = HF_OUT_OF_MEMORY;
	}
	HF_CriticalEnd(status);

	return i;
}
/*
fork()

not like the "real" fork on unix, since we don't have the process abstraction.
create another task, and copy all context (including the stack) from the function
caller to the new created task. fork() has to take care of some other stuff, such as
pointers to the stack based on some address math.

FIXME: FIFO and other stuff related to best effort tasks.
*/
int32_t HF_Fork(void){
	uint32_t status, i;
	int32_t task_id, id, rc;
	uint16_t period, capacity, deadline;
	uint32_t original_sp, new_sp, bytes_sp;

	status = HF_CriticalBegin();
	period = HF_task_entry->period;
	capacity = HF_task_entry->capacity;
	deadline = HF_task_entry->deadline;
	id = HF_CurrentTaskId();
	HF_task_entry = &HF_task[id];
	if (HF_task_entry->period == 0)
		task_id = HF_AddTask(HF_task_entry->ptask, HF_task_entry->description, HF_task_entry->stack_size, HF_task_entry->niceness, HF_task_entry->type);
	else
		task_id = HF_AddPeriodicTask(HF_task_entry->ptask, HF_task_entry->period, HF_task_entry->capacity, HF_task_entry->deadline, HF_task_entry->description, HF_task_entry->stack_size, HF_task_entry->niceness, HF_task_entry->type);
	HF_task_entry = &HF_task[id];
	if (task_id > 0){
		HF_task[task_id].period = period;
		HF_task[task_id].capacity = capacity;
		HF_task[task_id].capacity_counter = 0;
		HF_task[task_id].deadline = deadline;
		HF_task[task_id].next_tick_count = HF_tick_count + HF_task[task_id].period;
		HF_task[task_id].ticks++;
		HF_task[task_id].deadline_counter = HF_task_entry->priority;
		HF_task[task_id].status = TASK_BLOCKED;

//		HF_CriticalEnd(status);
		rc = setjmp(HF_task_entry->task_context);

		if (rc){
			HF_CriticalEnd(status);
			return HF_OK;
		}

//		status = HF_CriticalBegin();
		for(i=0;i<CONTEXT_SIZE;i++)
			HF_task[task_id].task_context[i] = HF_task_entry->task_context[i];

		original_sp = (uint32_t)HF_task_entry->stack + (HF_task_entry->stack_size - 4);
		bytes_sp = original_sp - HF_task_entry->task_context[CONTEXT_SP];
		new_sp = (uint32_t)HF_task[task_id].stack + (HF_task_entry->stack_size - 4) - bytes_sp;

//		printf("\nbottom_sp=%x, sp=%x, bytes_sp=%x, new_sp=%x", original_sp, HF_task[task_id].task_context[CONTEXT_SP], bytes_sp, new_sp);
		HF_task[task_id].task_context[CONTEXT_SP] = new_sp;
		HF_task[task_id].status = TASK_READY;

		for(i=0;i<HF_task_entry->stack_size;i++)
			HF_task[task_id].stack[i] = HF_task_entry->stack[i];

		HF_CriticalEnd(status);
		return task_id;
	}else{
		HF_CriticalEnd(status);
		return task_id;							// there is an error code here
	}
}


uint32_t HF_MemoryUsage(void){
	return HEAP_SIZE - HF_free_mem;
}

uint32_t HF_FreeMemory(void){
	return HF_free_mem;
}

/*
idle task
this task has index 0 (its the first one added, during HF_Init()).
*/
void IdleTask(void){					// system idle task

	printf("KERNEL: Idle task\n");

	for(;;){
//		HF_Halt();
	}
}

/*
OS initialization
*/
static void HF_Init(void){				// OS init procedure
	uint8_t i;
	uint32_t j, text, data, bss;
	extern int8_t SCHEDULING_POLICY_STRING[];
	extern int32_t _entry;
	extern int32_t _end;
	extern int32_t _etext;
	extern int32_t __bss_start;

	text = (uint32_t)&_etext - (uint32_t)&_entry;
	data = (uint32_t)&__bss_start - (uint32_t)&_etext;
	bss = (uint32_t)&_end - (uint32_t)&__bss_start;
#ifdef DEBUG
	printf("\n===========================================================");
	printf("\nHellfireOS RT Lite v1.61");
	printf("\n%s, %s (gcc %s)", __DATE__, __TIME__, __VERSION__);
	printf("\nEmbedded Systems Group - GSE, PUCRS - [2007 - 2015]");
	printf("\n===========================================================");

	printf("\nCPU ID:                %d", CPU_ID);
	printf("\nArchitecture:          %s", CPU_ARCH);
	printf("\nClock frequency:       %d kHz", CPU_SPEED/1000);
	printf("\nSystem tick time:      %d us", TICK_TIME_PERIOD);
	printf("\nTCB entry size:        %d bytes", sizeof(tcb));
	printf("\nTCB size:              %d bytes", sizeof(HF_task));
	printf("\nHeap size:             %d bytes", sizeof(HF_heap));
#ifdef NOC_INTERCONNECT
	printf("\nPacket buffer size:    %d flits in sw FIFOs (each task), %d flits in / out hw FIFOs", HF_PACKET_SLOTS * PACKET_SIZE, PACKET_SIZE);
#endif
	printf("\nMaximum tasks:         %d", MAX_TASKS);
	printf("\nMemory info");
	printf("\n    .text              %d bytes (%x - %x)", text, (uint32_t)&_entry, (uint32_t)&_etext);
	printf("\n    .data              %d bytes (%x - %x)", data, (uint32_t)&_etext, (uint32_t)&__bss_start);
	printf("\n    .bss               %d bytes (%x - %x)", bss, (uint32_t)&__bss_start, (uint32_t)&_end);
	printf("\nSystem memory usage:   %d bytes", text + data + bss);
	printf("\nKERNEL: initializing...");
#endif
	HF_HeapInit(HF_heap, sizeof(HF_heap));			// initialize system heap

	for(i=0;i<MAX_TASKS;i++){				// clear tcb entries
		HF_task_entry = &HF_task[i];
		HF_task_entry->id = 0;
		for(j=0;j<TASK_DESCRIPTION_SIZE;j++)
			HF_task_entry->description[j] = '\0';
		HF_task_entry->status = TASK_NOT_RUN;
		HF_task_entry->ticks = 0;
		HF_task_entry->last_tick_time = 0;
		HF_task_entry->priority = 0;
		HF_task_entry->period = 0;
		HF_task_entry->capacity = 0;
		HF_task_entry->capacity_counter = 0;
		HF_task_entry->deadline = 0;
		HF_task_entry->deadline_counter = 0;
		HF_task_entry->next_tick_count = 0;
		HF_task_entry->deadline_misses = 0;
		HF_task_entry->niceness = 0;
		HF_task_entry->niceness_counter = 0;
		for(j=0;j<CONTEXT_SIZE;j++)
			HF_task_entry->task_context[j] = 0;
		HF_task_entry->stack_ptr = 0;
		HF_task_entry->ptask = 0;
		HF_task_entry->stack = 0;
		HF_task_entry->stack_size = 0;
		HF_SemInit(&HF_task_entry->ipc_message_wait_send, 0);
		HF_SemInit(&HF_task_entry->ipc_message_wait_recv, 0);
#ifdef NOC_INTERCONNECT
		HF_task_entry->in_packet = 0;
		HF_task_entry->out_packet = 0;
		HF_task_entry->packets_sent = 0;
		HF_task_entry->packets_received = 0;
		HF_task_entry->packets_lost = 0;
		HF_task_entry->packets_seqerror = 0;
#endif
	}

	HF_tasks = 0;						// no tasks in system
	HF_current_task = 0;
	HF_max_index = 0;
	HF_be = 0;
	HF_tick_count = 0;					// clear OS ticks (so, on interrupt it will be 0)
	HF_context_switch_cycles = 0;				// clear last context switch cycles

	HF_AddServices();
}

/*
OS startup

this bloody bastard is called after the OS has been set up and tasks were added.
it initializes the interrupt trap and clears the interrupt mask register. interrupt
handling routines are registered, the interrupt mask is set and interrupts are
enabled. so, we sit there and wait..
*/
void HF_Start(void){						// OS start procedure
#ifdef DEBUG
	printf("\nKERNEL: free heap: %d bytes", HF_FreeMemory());
#endif
	srand((uint32_t)CPU_ID);
	HF_HardwareInit();

	IdleTask();

//	HF_current_task = 0;
//	HF_task_entry = &HF_task[HF_current_task];
//	HF_task_entry->status = TASK_RUNNING;
//	HF_task_entry->ticks++;
//	HF_RestoreExecution(HF_task_entry->task_context, 1, HF_current_task);

	for(;;);						// we should never reach this loop
}

/*
this is our entry point. we initialize the OS and call ApplicationMain().
pretty straightforward. of course, we should never reach the end of this
function.
*/
int32_t HF_Main(int32_t argc, int8_t **argv){
	static uint32_t oops=1;					// this is an UGLY hack to prevent from rebooting
								// in cycles after a GPF, but it works flawlessly
	HF_BasicHardwareInit();
	if (oops == 1){						// this should never be different than 1
		oops = 0;					// yeah. next time generate a GPF.
		HF_Init();
		ApplicationMain();
		HF_Panic(PANIC_ABORTED);			// ... you forgot to start the kernel from the application!
	}else{							// yep. that's all we can do when you screw things up.
		HF_Panic(PANIC_GPF);
	}
	
	return HF_OK;
}

