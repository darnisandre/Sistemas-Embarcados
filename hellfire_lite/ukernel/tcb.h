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
file: tcb.h
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 12/2008
last revision: 05/2014
function: task control block structures variables
=====================================================================
definition of the task control block structure
=====================================================================
*/

/* task status definitions */
#define TASK_READY			0		// task has run, and is ready to run again
#define TASK_RUNNING			1		// task is running (only one task/core can be in this state)
#define TASK_BLOCKED			2		// task is blocked, and stopped executing. can be resumed later.
#define TASK_WAITING			3		// task is waiting on a semaphore, and will be resumed soon
#define TASK_NOT_RUN			4		// task is not ready, since it has never run
#define TASK_DEFAULT			0
#define TASK_ONESHOT			3

/* best effort priorities */
#define KERNEL_LOWMIN_NICENESS		16
#define KERNEL_LOWMAX_NICENESS		63
#define KERNEL_HIGHMIN_NICENESS		64
#define KERNEL_HIGHMAX_NICENESS		127
#define USER_LOWMIN_NICENESS		128
#define USER_LOWMAX_NICENESS		159
#define USER_HIGHMIN_NICENESS		160
#define USER_HIGHMAX_NICENESS		223
#define USER_IDLEMIN_NICENESS		224
#define USER_IDLEMAX_NICENESS		255
#define DEFAULT_NICENESS		128

/* generic kernel panic definitions */
#define PANIC_ABORTED			0x00
#define PANIC_GPF			0x01
#define PANIC_STACK_OVERFLOW		0x02
#define PANIC_NO_TASKS_LEFT		0x03
#define PANIC_HW_FAULT			0x04
#define PANIC_UNKNOWN			0xff

/*
Hellfire generic internal prototypes
*/
void HF_Halt(void);
void HF_BasicHardwareInit(void);
void HF_HardwareInit(void);
void HF_UpdateCounterMask(void);
void HF_Panic(int32_t cause);
void HF_DispatcherISR(void *arg);
uint8_t HF_TaskReschedule(void);
void HF_HeapInit(void *heap, uint32_t len);
void ApplicationMain(void);

/*
the task control block
*/
typedef struct{
	uint8_t id;					// task id
	int8_t description[TASK_DESCRIPTION_SIZE];	// task description (or name)
	uint8_t status;					// 0 - ready,  1 - running,  2 - blocked, 3 - waiting, 4 - not run, 5 - overrun
	uint32_t ticks;					// total task ticks executed
	uint32_t last_tick_time;			// last tick duration (in us)
	uint16_t priority;				// task priority (used on RT schedulers)
	uint16_t period;				// task period
	uint16_t capacity;				// task capacity
	uint16_t capacity_counter;			// capacity counter (decremented each executed tick)
	uint16_t deadline;				// task deadline
	uint16_t deadline_counter;			// task deadline counter (decremented at each scheduling round)
	uint32_t next_tick_count;			// task realtime deadline estimation
	uint32_t deadline_misses;			// task realtime deadline misses
	uint8_t niceness;				// task niceness. used by the BE scheduler.
	uint8_t niceness_counter;			// task niceness (down counter) 
	uint8_t type;					// task type (TASK_DEFAULT, TASK_CAN_MIGRATE, TASK_CANNOT_MIGRATE, TASK_ONESHOT)
	context task_context;				// task context
	void (*ptask) (void);				// pointer to task entry point
	void (*stack_ptr) (void);			// pointer to task stack address (bottom)
	uint8_t *stack;					// pointer to task stack buffer
	uint32_t stack_size;				// task stack size
	uint8_t ipc_message_source;
	uint16_t ipc_message_size;
	void *ipc_message_ptr;
	semaphore ipc_message_wait_send;		// inter-task message lock
	semaphore ipc_message_wait_recv;		// inter-task message lock
#ifdef NOC_INTERCONNECT
	volatile uint8_t out_packet;			// packet ready to be sent
	packet_out_buffer_type *packet_out;		// pointer to outgoing packet data
	volatile uint8_t in_packet;			// packets received (on queue)
	packet_in_buffer_type *packet_in;		// pointer to incoming packet data
	volatile uint8_t comm_busy;			// task is busy communicating
	volatile uint8_t comm_ack;
	uint32_t packets_sent;
	uint32_t packets_received;
	uint32_t packets_lost;
	uint32_t packets_seqerror;
#endif
	uint32_t tcb_signature;
} tcb;


tcb HF_task[MAX_TASKS];						// global vector of task control blocks
tcb *HF_task_entry;						// pointer to a task control block
uint32_t HF_tick_count;						// system tick counter (overflow in aprox. 521 days, 10.48572ms tick)
uint32_t HF_context_switch_cycles;				// cycles spent on last context switch
uint8_t HF_tasks, HF_current_task, HF_last_task, HF_max_index, HF_be, HF_schedule;	// general OS control variables
#ifndef DATA_HEAP
uint8_t HF_heap[HEAP_SIZE];					// system heap
#endif
uint32_t HF_free_mem;

