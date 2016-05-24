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
file: ipc.c
author: sergio johann filho [sergiojohannfilho@gmail.com]
date: 08/2011
last revision: 08/2011
function: interprocess communication
=====================================================================
interprocess (in our case task) communication facilities
=====================================================================
*/

#include <prototypes.h>

/*
mailbox implementation
*/
void HF_MboxInit(mailbox *mbox, uint8_t n_waiting_tasks){
	uint32_t status;

	status = HF_CriticalBegin();
	mbox->msg = NULL;
	mbox->n_waiting_tasks = n_waiting_tasks;
	mbox->count = n_waiting_tasks;
	HF_SemInit(&mbox->msend, 1);
	HF_SemInit(&mbox->mrecv, 0);
	HF_CriticalEnd(status);
}

void HF_MboxSend(mailbox *mbox, void *msg){
	uint32_t i;

	for(i=0; i<mbox->n_waiting_tasks; i++){
		HF_SemWait(&mbox->msend);
		mbox->msg = msg;
		HF_SemPost(&mbox->mrecv);
	}
}

void *HF_MboxRecv(mailbox *mbox){
	void *msg;

	HF_SemWait(&mbox->mrecv);
	msg = mbox->msg;
	mbox->count--;
	if (mbox->count == 0){
		mbox->msg = NULL;
		mbox->count = mbox->n_waiting_tasks;
	}
	HF_SemPost(&mbox->msend);

	return msg;
}

void *HF_MboxAccept(mailbox *mbox){
	uint32_t status;
	void *msg;

	status = HF_CriticalBegin();
	msg = mbox->msg;
	if (msg != NULL){
		mbox->count--;
		if (mbox->count == 0){
			mbox->msg = NULL;
			mbox->count = mbox->n_waiting_tasks;
		}
		HF_SemPost(&mbox->msend);
	}
	HF_CriticalEnd(status);

	return msg;
}

/*
local send() and receive() implementation
TODO: use wrappers on HF_Send() and HF_Receive() to make the message passing implementation generic.
*/
int32_t HF_MsgSend(uint8_t target_id, uint8_t buf[], uint16_t size){
	uint32_t status;
	uint16_t p, c, d;
	uint8_t id;

	HF_task[target_id].ipc_message_source = HF_CurrentTaskId();
	HF_task[target_id].ipc_message_size = size;
	HF_task[target_id].ipc_message_ptr = buf;
	HF_SemPost(&HF_task[target_id].ipc_message_wait_send);
	HF_SemWait(&HF_task[target_id].ipc_message_wait_recv);
	return HF_OK;
}

int32_t HF_MsgRecv(uint8_t *source_id, uint8_t buf[], uint16_t *size){
	uint32_t status;
	uint16_t p, c, d;
	uint32_t i;
	uint8_t id;
	void *msg;
	uint8_t *buf2;

	id = HF_CurrentTaskId();

	HF_SemWait(&HF_task[id].ipc_message_wait_send);
	*source_id = HF_task[id].ipc_message_source;
	*size = HF_task[id].ipc_message_size;
	msg = HF_task[id].ipc_message_ptr;
	buf2 = msg;
	for(i=0; i<*size; i++)
		buf[i] = buf2[i];
	HF_SemPost(&HF_task[id].ipc_message_wait_recv);
	return HF_OK;
}

