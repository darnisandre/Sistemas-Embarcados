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
file: mpsoc.c
author: sergio johann filho [sergio.johann@acad.pucrs.br]
date: 07/2008
last revision: 12/2013
	- Oliver Longhi (bug fixes)
function: packet driver, low level network access
=====================================================================
network interface drivers. this implementation follows the hardware
protocol created for a NoC interface / wrapper. if we keep the same
protocol and packet format, this driver can be used for other media,
such as buses.
=====================================================================
*/

#include <prototypes.h>

#ifdef NOC_INTERCONNECT

semaphore pktdrv_mutex;

uint16_t HF_Core(uint16_t core_n){
#ifndef BUS
	return (0x0000 | ((NOC_COLUMN(core_n) << 4) | NOC_LINE(core_n)));
#else
	return core_n;
#endif
}

uint32_t HF_PacketsSent(uint8_t id){
	return HF_task[id].packets_sent;
}

uint32_t HF_PacketsReceived(uint8_t id){
	return HF_task[id].packets_received;
}

uint8_t HF_PacketsQueued(uint8_t id){
	return HF_task[id].in_packet;
}

uint32_t HF_PacketsLost(uint8_t id){
	return HF_task[id].packets_lost;
}

uint32_t HF_PacketsSeqError(uint8_t id){
	return HF_task[id].packets_seqerror;
}

static inline uint16_t NocStatus(void){
	uint16_t status;

	status = (uint16_t)MemoryRead(NOC_STATUS);
	asm ("nop\nnop\nnop");
	return status;
}

static inline uint16_t NocRead(void){
	uint16_t data;

	data = (uint16_t)MemoryRead(NOC_READ);
	asm ("nop\nnop\nnop");
	return data;
}

static inline void NocWrite(uint16_t data){
	MemoryWrite(NOC_WRITE, data);
	asm ("nop\nnop\nnop");
}

/*
RecvCommServiceISR (packet receive handler)

Part of the packet driver. This piece of code runs as an interrupt service routine, added during driver startup.
Data is taken from the network interface receive queue and put in the corrensponding task software buffer.
Minimal error handling is performed: messed up packet, software buffer full and stuff like that - drop it.
Due to hardware limitations (bugs i couldn't fix yet), we need to read ALL data from the incoming hw fifo
at once, or things get nasty. A solution was to flush the fifo and put data on task 0 sw buffer, then copy
to the correct task buffer after decoding.

(put a damn packet drawing here to clarify all this mess)

*/
void RecvCommServiceISR(void *arg){
	uint16_t target_cpu, payload, source_cpu, task, target_task, msg_size, seq, control, crc, crc_chk, l, end, n;
	uint32_t t, status, uid;
	int32_t m, s;
	uint16_t cpu;
	uint8_t id, st;

	status = HF_CriticalBegin();								// this IS NEEDED on the simulator!

	NocRead();										// start the NI...

	target_cpu = NocRead();									// start decoding the received packet
	payload = NocRead();
	source_cpu = NocRead();
	task = NocRead();
	msg_size = NocRead();
	seq = NocRead();
	control = NocRead();
	crc = NocRead();

	target_task = task & 0x00ff;

	if (payload != PACKET_SIZE - 2){
#ifdef DEBUG_FULL
		printf("\nKERNEL: packet is messed up. t_cpu: %d payload: %x s_cpu %d task: %x size: %d seq: %d", target_cpu, payload, source_cpu, task, msg_size, seq);
#endif
		for(l = 8; l < PACKET_SIZE; l++)
			NocRead();

		return;									// return to IRQ handler and hopefully to the interrupted task soon
	}

	if (HF_task[target_task].ptask){						// do we have a task in this id?
		if (HF_task[target_task].in_packet < HF_PACKET_SLOTS){			// is there a free slot on the software buffer?
			end = HF_task[target_task].packet_in->end;
			HF_task[target_task].packet_in->data[end + 0] = target_cpu;	// fill packet header
			HF_task[target_task].packet_in->data[end + 1] = payload;
			HF_task[target_task].packet_in->data[end + 2] = source_cpu;
			HF_task[target_task].packet_in->data[end + 3] = task;
			HF_task[target_task].packet_in->data[end + 4] = msg_size;
			HF_task[target_task].packet_in->data[end + 5] = seq;
			HF_task[target_task].packet_in->data[end + 6] = control;
			HF_task[target_task].packet_in->data[end + 7] = 0x0000;

			for(l = 8; l < PACKET_SIZE; l++)				// copy data to task software buffer
				HF_task[target_task].packet_in->data[end + l] = NocRead();
#ifdef NOC_CRC16
			crc_chk = crc16slow(HF_task[target_task].packet_in->data + end, PACKET_SIZE);
			if (crc != crc_chk){
				printf("\nKERNEL: packet CRC mismatch (got %x, chk %x)", crc, crc_chk);
				return;
			}
#endif
			HF_task[target_task].packet_in->end = (end + PACKET_SIZE) % (HF_PACKET_SLOTS * PACKET_SIZE);
			HF_task[target_task].in_packet++;				// packet ready to be processed =)
		}else{									// software buffer full... we must wait or drop the packet
#ifdef DEBUG
			printf("\nKERNEL: task %d (%d) - dropping packet...", target_task, HF_task[target_task].in_packet);
#endif
			for(l = 8; l < PACKET_SIZE; l++)
				NocRead();

			HF_task[target_task].packets_lost++;
		}
	}else{										// no task with this id... drop the packet
#ifdef DEBUG
		printf("\nKERNEL: no task with id %d (offender: cpu %d task %d) - dropping packet...", target_task, source_cpu, task >> 8);

		for(l = 8; l < PACKET_SIZE; l++)
			NocRead();
#endif
	}

	return;										// return to IRQ handler and hopefully to the interrupted task now
}

/*
HF_NB_Receive (non-blocking message receive driver)
FIXME: packet sequence handling is shitty as hell
*/
int32_t HF_NB_Receive(uint16_t *source_cpu, uint8_t *source_id, uint8_t buf[], uint16_t *size, uint32_t timeout){
	uint16_t target_cpu, cpu, payload, task, msg_size, seq, control, crc, start;
	uint16_t l, p=0, packet=0, packets, pad_bytes, payload_bytes;
	uint8_t task_id, error = HF_OK;
	uint32_t t, status;

	HF_task[HF_CurrentTaskId()].comm_busy = 1;
	task_id = HF_CurrentTaskId();							// get id from current task
	t = HF_TaskTicks(task_id);

	while(HF_task[task_id].in_packet==0){						// wait for a packet...
		if (((HF_TaskTicks(task_id) - t) >= timeout) && (timeout > 0)){
			HF_task[HF_CurrentTaskId()].comm_busy = 0;
			return HF_COMM_TIMEOUT;
		}
	}

	start = HF_task[task_id].packet_in->start;

	target_cpu = HF_task[task_id].packet_in->data[start + 0];
	payload = HF_task[task_id].packet_in->data[start + 1];
	cpu = HF_task[task_id].packet_in->data[start + 2];
	task = HF_task[task_id].packet_in->data[start + 3];
	msg_size = HF_task[task_id].packet_in->data[start + 4];
	seq = HF_task[task_id].packet_in->data[start + 5];
	control = HF_task[task_id].packet_in->data[start + 6];
	crc = HF_task[task_id].packet_in->data[start + 7];

	payload_bytes = (PACKET_SIZE - 8) * sizeof(uint16_t);
	(msg_size % payload_bytes == 0)?(packets = msg_size / payload_bytes):(packets = msg_size / payload_bytes + 1);
	pad_bytes = payload_bytes - (msg_size % payload_bytes);

	*source_cpu = cpu;
	*source_id = task >> 8;
	*size = msg_size;

	while(++packet < packets){
		if (HF_task[task_id].packet_in->data[start + 5] != seq++){
#ifdef DEBUG
			printf("\nKERNEL: packet sequence mismatch. recv: %d curr: %d (fifo: %d%%)", HF_task[task_id].packet_in->data[start + 5], seq, (HF_PacketsQueued(task_id) * 100) / HF_PACKET_SLOTS);
			HF_task[task_id].packets_seqerror++;
			error = HF_SEQ_ERROR;		
#endif
		}
		status = HF_CriticalBegin();
		for(l = 8; l < PACKET_SIZE; l++){
			buf[p++] = (uint8_t)(HF_task[task_id].packet_in->data[start + l] >> 8);
			buf[p++] = (uint8_t)(HF_task[task_id].packet_in->data[start + l] & 0xff);
		}
		start = (start + PACKET_SIZE) % (HF_PACKET_SLOTS * PACKET_SIZE);
		HF_task[task_id].packet_in->start = start;
		HF_task[task_id].in_packet--;
		HF_CriticalEnd(status);
		HF_task[task_id].packets_received++;

		while(HF_task[task_id].in_packet==0){					// wait for a packet...
			if (((HF_TaskTicks(task_id) - t) >= timeout) && (timeout > 0)){
				HF_task[HF_CurrentTaskId()].comm_busy = 0;
				return HF_COMM_TIMEOUT;
			}
		}
	}

	if (HF_task[task_id].packet_in->data[start + 5] != seq++){
#ifdef DEBUG
		printf("\nKERNEL: packet sequence mismatch. recv: %d curr: %d (fifo: %d%%)", HF_task[task_id].packet_in->data[start + 5], seq, (HF_PacketsQueued(task_id) * 100) / HF_PACKET_SLOTS);
		HF_task[task_id].packets_seqerror++;
		error = HF_SEQ_ERROR;
#endif
	}
	status = HF_CriticalBegin();
	for(l = 8; (l<PACKET_SIZE) && (p < msg_size); l++){
		buf[p++] = (uint8_t)(HF_task[task_id].packet_in->data[start + l] >> 8);
		buf[p++] = (uint8_t)(HF_task[task_id].packet_in->data[start + l] & 0xff);
	}
	start = (start + PACKET_SIZE) % (HF_PACKET_SLOTS * PACKET_SIZE);
	HF_task[task_id].packet_in->start = start;
	HF_task[task_id].in_packet--;
	HF_CriticalEnd(status);
	HF_task[task_id].packets_received++;

	HF_task[HF_CurrentTaskId()].comm_busy = 0;

	return error;
}

/*
HF_Receive (message receive driver)

Part of the packet driver. Data is taken from task software queue. The RecvCommService part of the driver
executes as an interrupt handler, and will put data there. Data is split into packets, and must be reconstructed.
Target core, payload, task and message size must be removed from headers, so do padding bytes on last packet.
FIXME: implement the fucking flow control!
*/
int32_t HF_Receive(uint16_t *source_cpu, uint8_t *source_id, uint8_t buf[], uint16_t *size){
	while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
	return HF_NB_Receive(source_cpu, source_id, buf, size, 0);
}

/*
HF_NB_Send (non-blocking message send driver)
FIXME: packet sequence handling is shitty as hell
*/
int32_t HF_NB_Send(uint16_t target_cpu, uint8_t target_id, uint8_t buf[], uint16_t size, uint32_t timeout){
	uint16_t source_cpu, payload, task, control, crc, k;
	uint16_t l, p=0, packet=0, packets, pad_bytes, payload_bytes;
	uint8_t task_id;
	uint32_t t, status;
	// flow control vars
	uint16_t ack_source_cpu, ack_task_id, ack_payload;

	// you are trying to send more data than the sw queue on the target task can hold, even on a perfect scenario.
	// being wrong, nuts, or just plain stupid yields the same result.
	if (((size / ((PACKET_SIZE * 2) - (HF_PACKET_HDR_SIZE * 2))) + 1) > HF_PACKET_SLOTS)
		return HF_COMM_UNFEASIBLE;

	HF_SemWait(&pktdrv_mutex);							// only one task can access the output queue at a time

	HF_task[HF_CurrentTaskId()].comm_busy = 1;
	task_id = HF_CurrentTaskId();							// get id from current task
	payload = PACKET_SIZE - 2;							// NoC payload = number of flits - target - payload
	task = (task_id << 8) | target_id;						// merge current task id and target task id in one flit

	payload_bytes = (PACKET_SIZE - 8) * sizeof(uint16_t);				// actual payload size in bytes, not including headers
	(size % payload_bytes == 0)?(packets = size / payload_bytes):(packets = size / payload_bytes + 1);
	pad_bytes = payload_bytes - (size % payload_bytes);				// number pad bytes on last packet

	t = HF_TaskTicks(task_id);

	while(++packet < packets){							// packetize raw data
		HF_task[task_id].packet_out->data[0] = target_cpu;			// fill header
		HF_task[task_id].packet_out->data[1] = payload;				// full packet
		HF_task[task_id].packet_out->data[2] = HF_CurrentCpuId();
		HF_task[task_id].packet_out->data[3] = task;
		HF_task[task_id].packet_out->data[4] = size;
		HF_task[task_id].packet_out->data[5] = packet;
		HF_task[task_id].packet_out->data[6] = 0x0000;
		HF_task[task_id].packet_out->data[7] = 0x0000;

		for(l = 8; l < PACKET_SIZE; l++){					// fill packet with msg data
			HF_task[task_id].packet_out->data[l] = (buf[p] << 8) | buf[p+1];// align bytes to 16 bit flits
			p+=2;
		}

/* CRC16 */
#ifdef NOC_CRC16
		HF_task[task_id].packet_out->data[7] = crc16slow(HF_task[task_id].packet_out->data, PACKET_SIZE);
#endif
/* end of CRC16 */

		while((NocStatus() & 0x1) == 0){					// wait for the interface...
			if (((HF_TaskTicks(task_id) - t) >= timeout) && (timeout > 0)){
				HF_task[HF_CurrentTaskId()].comm_busy = 0;
				HF_SemPost(&pktdrv_mutex);
				return HF_IF_NOT_READY;
			}
		}

		status = HF_CriticalBegin();						// we must put data on the queue without being disturbed
		for(l = 0; l < PACKET_SIZE; l++)					// fill hw queue with msg data
			NocWrite(HF_task[task_id].packet_out->data[l]);
		HF_CriticalEnd(status);

		HF_task[task_id].packets_sent++;
	}

	HF_task[task_id].packet_out->data[0] = target_cpu;				// fill header
	HF_task[task_id].packet_out->data[1] = payload;					// full packet
	HF_task[task_id].packet_out->data[2] = HF_CurrentCpuId();
	HF_task[task_id].packet_out->data[3] = task;
	HF_task[task_id].packet_out->data[4] = size;
	HF_task[task_id].packet_out->data[5] = packet;
	HF_task[task_id].packet_out->data[6] = 0x0000;
	HF_task[task_id].packet_out->data[7] = 0x0000;

	for(l = 8; (l<PACKET_SIZE) && (p < size); l++){
		HF_task[task_id].packet_out->data[l] = (buf[p] << 8) | buf[p+1];	// align bytes to 16 bit flits
		p+=2;
	}
	for(; l < PACKET_SIZE; l++)
		HF_task[task_id].packet_out->data[l] = 0xdead;				// this is our garbage mark

/* CRC16 */
#ifdef NOC_CRC16
	HF_task[task_id].packet_out->data[7] = crc16slow(HF_task[task_id].packet_out->data, PACKET_SIZE);
#endif
/* end of CRC16 */

	while((NocStatus() & 0x1) == 0){						// wait for the interface...
		if (((HF_TaskTicks(task_id) - t) >= timeout) && (timeout > 0)){
			HF_task[HF_CurrentTaskId()].comm_busy = 0;
			HF_SemPost(&pktdrv_mutex);
			return HF_IF_NOT_READY;
		}
	}

	status = HF_CriticalBegin();							// we must put data on the queue without being disturbed
	for(l = 0; l < PACKET_SIZE; l++)							// fill hw queue with msg data
		NocWrite(HF_task[task_id].packet_out->data[l]);
	HF_CriticalEnd(status);

	HF_task[task_id].packets_sent++;
	HF_task[HF_CurrentTaskId()].comm_busy = 0;

	HF_SemPost(&pktdrv_mutex);

	return HF_OK;
}

/*
HF_Send (message send driver)
FIXME: implement the fucking flow control!
*/
int32_t HF_Send(uint16_t target_cpu, uint8_t target_id, uint8_t buf[], uint16_t size){
	return HF_NB_Send(target_cpu, target_id, buf, size, 0);
}

/*
HF_NetworkInterfaceInit

Registers the network interface IRQ line, sets interrupt mask and adds the SendCommService task.
*/
void HF_NetworkInterfaceInit(void){
	uint32_t i;

	HF_SemInit(&pktdrv_mutex, 1);

	for(i = 0; i < PACKET_SIZE; i++)
		NocRead();
	HF_InterruptRegister(IRQ_NOC_READ, (HF_funcptr)RecvCommServiceISR);
	HF_InterruptMaskSet(IRQ_NOC_READ);
#ifdef DEBUG
	printf("\nKERNEL: network interface driver registered");
#endif
}

/*
HF_QueryTaskId()
*/
int32_t HF_QueryTaskId(uint16_t target_cpu, int8_t *description){
	int32_t error;
	uint16_t source_cpu, size;
	uint8_t source_id;

	typedef union{
		uint8_t buf[TASK_DESCRIPTION_SIZE];
		int32_t task_id;
	} query_msg;

	query_msg msg;
	

	strcpy(msg.buf, description);
	error = HF_NB_Send(target_cpu, 1, msg.buf, TASK_DESCRIPTION_SIZE, 5);
	if (error)
		return error;
	HF_TaskYield();
	error = HF_NB_Receive(&source_cpu, &source_id, msg.buf, &size, 100);
	if (error)
		return error;

	return msg.task_id;	
}

int32_t HF_NB_QueryTaskId(uint16_t target_cpu, int8_t *description){
	int32_t error;
	uint16_t source_cpu, size;
	uint8_t source_id;

	typedef union{
		uint8_t buf[TASK_DESCRIPTION_SIZE];
		int32_t task_id;
	} query_msg;

	query_msg msg;
	

	strcpy(msg.buf, description);
	error = HF_NB_Send(target_cpu, 1, msg.buf, TASK_DESCRIPTION_SIZE, 5);
	if (error)
		return error;

	return HF_OK;
}

int32_t HF_NB_RecvTaskId(uint16_t *cpu, int32_t *tid){
	int32_t error;
	uint16_t source_cpu, size;
	uint8_t source_id;

	typedef union{
		uint8_t buf[TASK_DESCRIPTION_SIZE];
		int32_t task_id;
	} query_msg;

	query_msg msg;
	
	error = HF_NB_Receive(&source_cpu, &source_id, msg.buf, &size, 100);
	if (error)
		return error;

	*cpu = source_cpu;
	*tid = msg.task_id;

	return HF_OK;	
}

void SystemMonitor(void){
	int32_t error, id;
	uint16_t source_cpu, size;
	uint8_t source_id;

	typedef union{
		uint8_t buf[TASK_DESCRIPTION_SIZE];
		int32_t task_id;
	} query_msg;

	query_msg msg;

	for (;;){
		HF_Receive(&source_cpu, &source_id, msg.buf, &size);
#ifdef DEBUG
		printf("\nKERNEL: query for task '%s', cpu %d task %d", msg.buf, source_cpu, source_id);
#endif
		id = HF_TaskId(msg.buf);
		msg.task_id = id;
		HF_NB_Send(HF_Core(source_cpu), source_id, msg.buf, TASK_DESCRIPTION_SIZE, 5);
	}
}
#endif


