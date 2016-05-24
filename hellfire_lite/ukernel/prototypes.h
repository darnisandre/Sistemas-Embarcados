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
last revision: 03/2014
function: definitions and function prototypes
=====================================================================
enough said.
=====================================================================
*/

/*
include default stuff
*/
#include <hal.h>
#include <hflibc.h>
#if N_CORES > 1
#include <mpsoc.h>
#endif
#include <list.h>
#include <mutex.h>
#include <ipc.h>
#include <tcb.h>

/*
error codes
*/
/* generic */
#define	HF_OK			0
#define HF_ERROR		-1
/* task errors */
#define HF_INVALID_ID		-100
#define HF_INVALID_PARAMETER	-101
#define HF_INVALID_STATE	-102
#define HF_EXCEED_MAX_NUM	-103
#define HF_OUT_OF_MEMORY	-104
#define HF_INVALID_NAME		-105
/* communication errors */
#define HF_INVALID_CPU		-106
#define HF_IF_NOT_READY		-107
#define HF_COMM_TIMEOUT		-108
#define HF_SEQ_ERROR		-109
#define HF_COMM_BUSY		-110
#define HF_COMM_UNFEASIBLE	-111
#define HF_COMM_ERROR		-112

/*
the Hellfire API
*/
uint32_t HF_TaskTicks(uint8_t id);
uint32_t HF_TaskEnergyUsage(uint8_t id);
uint32_t HF_TaskMemoryUsage(uint8_t id);
uint32_t HF_LastTickTime(uint8_t id);
uint32_t HF_LastContextSwitchTime(void);
int32_t HF_TaskId(int8_t *description);						// new!
uint8_t HF_CurrentTaskId(void);
uint8_t HF_CurrentCpuId(void);
uint32_t HF_CurrentCpuFrequency(void);
uint16_t HF_NCores(void);
uint8_t HF_NTasks(void);
int32_t HF_BlockTask(uint8_t id);
int32_t HF_ResumeTask(uint8_t id);
int32_t HF_KillTask(uint8_t id);
int32_t HF_AddTask(void (*task)(), int8_t description[], uint32_t stack_size, uint8_t niceness, uint8_t type);
int32_t HF_Fork(void);
uint32_t HF_MemoryUsage(void);
uint32_t HF_FreeMemory(void);
void HF_TaskYield(void);
void HF_Start(void);

int32_t HF_ListInit(list_t *list);
int32_t HF_ListAppend(list_t *list, void *item);
int32_t HF_ListInsert(list_t *list, void *item, int32_t pos);
int32_t HF_ListRemove(list_t *list, int32_t pos);
int32_t HF_ListMove(list_t *list, int32_t src_pos, int32_t dst_pos);
void *HF_ListGet(list_t *list, int32_t pos);
int32_t *HF_ListCount(list_t *list);

void HF_EnterRegion(mutex *m);
void HF_LeaveRegion(mutex *m);
void HF_SemInit(semaphore *s, int32_t value);
void HF_SemWait(semaphore *s);
void HF_SemPost(semaphore *s);

void HF_MboxInit(mailbox *mbox, uint8_t n_waiting_tasks);
void HF_MboxSend(mailbox *mbox, void *msg);
void *HF_MboxRecv(mailbox *mbox);
void *HF_MboxAccept(mailbox *mbox);
int32_t HF_MsgSend(uint8_t target_id, uint8_t buf[], uint16_t size);				// TODO: deprecate!
int32_t HF_MsgRecv(uint8_t *source_id, uint8_t buf[], uint16_t *size);				// TODO: deprecate!

int32_t HF_Send(uint16_t target_cpu, uint8_t target_id, uint8_t buf[], uint16_t size);		// TODO: msg context last arg! (-1 as *any*)
int32_t HF_Receive(uint16_t *source_cpu, uint8_t *source_id, uint8_t buf[], uint16_t *size);	// TODO: msg context last arg! (-1 as *any*)
int32_t HF_NB_Send(uint16_t target_cpu, uint8_t target_id, uint8_t buf[], uint16_t size, uint32_t timeout); // TODO: msg context as last arg! (-1 as *any*)
int32_t HF_NB_Receive(uint16_t *source_cpu, uint8_t *source_id, uint8_t buf[], uint16_t *size, uint32_t timeout); // TODO: msg context as last arg! (-1 as *any*)
int32_t HF_QueryTaskId(uint16_t target_cpu, int8_t *description);		// new!!
int32_t HF_NB_QueryTaskId(uint16_t target_cpu, int8_t *description);		// new!!
int32_t HF_NB_RecvTaskId(uint16_t *cpu, int32_t *tid);				// new!!
uint16_t HF_Core(uint16_t core_n);
uint32_t HF_PacketsSent(uint8_t id);
uint32_t HF_PacketsReceived(uint8_t id);
uint8_t HF_PacketsQueued(uint8_t id);	// TODO: msg context as a second arg (-1 as *any*)!
uint32_t HF_PacketsLost(uint8_t id);
uint32_t HF_PacketsSeqError(uint8_t id);

int32_t HF_AddPeriodicTask(void (*task)(), uint16_t period, uint16_t capacity, uint16_t deadline, int8_t description[], uint32_t stack_size, uint8_t niceness, uint8_t type);

