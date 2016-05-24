#include <prototypes.h>

void task_send(void){
	uint32_t i, j, k, s;
	uint16_t p, e, d, m = TASK_DEFAULT;
	uint8_t buf[300];
	uint8_t buf2[] = "test";
	uint8_t buf3[30];
	uint32_t status, cycles;

	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;

	for(i=0;i<N_CORES;i++)
		printf("\nCore %d, mapped to router %x", i, HF_Core(i));

	while(1){
		for (k = 0; k < 5; k++){
			printf("\n\nCommunications performance test iteration #%d", k);

			printf("\n\nTest #1 - performance many to many");
			for(i=32; i<=192; i+=32){
				printf("\nsend: %d bytes msg to all processors... ", i);
				cycles = MemoryRead(COUNTER_REG);
				for(j=0; j<6; j++){
					if (j != CPU_ID){
						strcpy(buf, "hello from CPU ");
						itoa(CPU_ID, buf3, 10);
						strcat(buf, buf3);
						s = HF_Send(HF_Core(j), CPU_ID + 3, buf, i);
						if (s) printf("\nerror sending!");
					
					}
				}
				cycles = MemoryRead(COUNTER_REG) - cycles;
				printf("%d cycles, %d Kbps", cycles, ((i * 5 * 8 * 1000) / (cycles / (CPU_SPEED/1000000))));
				HF_TaskYield();
			}
		}

		for(j=1; j<6; j++){
			s = HF_Send(HF_Core(j), 2, buf2, sizeof(buf2));
			if (s) printf("\nerror sending!");
		}

		sleep(500);
		HF_Panic(0);
	}
}

void task_recv(void){
	uint16_t source_cpu, status;
	uint8_t source_id;
	uint16_t size;
	// messages from core 0 are big and we must have room to put them.
	uint8_t buf[300];

	while(1){
		HF_Receive(&source_cpu, &source_id, buf, &size);
		printf("\nrecv size: %d loss: %d", size, HF_PacketsLost(HF_CurrentTaskId()));
		printf("\nmsg: %s", buf);
		if (strcmp(buf, "test") == 0){
			sleep(10);
			HF_Panic(0);
		}
	}
}

void ApplicationMain(void){
	HF_AddTask(task_send, "sender", 2048, 1, TASK_DEFAULT);

	HF_AddTask(task_recv, "receiver", 2048, 1, TASK_DEFAULT);
	HF_AddTask(task_recv, "receiver", 2048, 1, TASK_DEFAULT);
	HF_AddTask(task_recv, "receiver", 2048, 1, TASK_DEFAULT);
	HF_AddTask(task_recv, "receiver", 2048, 1, TASK_DEFAULT);
	HF_AddTask(task_recv, "receiver", 2048, 1, TASK_DEFAULT);
	HF_AddTask(task_recv, "receiver", 2048, 1, TASK_DEFAULT);

	HF_Start();
}
