// software queues with 16 slots are enough for 1536 byte messages!

#include <prototypes.h>

#if CPU_ID == 0
void task_send(void){
	uint32_t i, j, k, s;
	uint16_t p, e, d, m = TASK_DEFAULT;
	uint8_t buf[2000] = "hello world from processor 0! \\o/";
	uint8_t buf2[] = "test";
	uint32_t status, cycles;

	for(i=0;i<N_CORES;i++)
		printf("\nCore %d, mapped to router %x", i, HF_Core(i));

	while(1){
		printf("\n\nCommunications performance test");

		printf("\n\nTest #1 - performance 1 to 1");
		for(i=50; i<=1000; i+=50){
			printf("\nSending a %d byte payload message... ", i);
			cycles = MemoryRead(COUNTER_REG);
			s = HF_Send(HF_Core(1), 2, buf, i);
			if (s) printf("\nerror sending!");
			cycles = MemoryRead(COUNTER_REG) - cycles;
			printf("%d cycles, %d Kbps", cycles, ((i * 8 * 1000) / (cycles / (CPU_SPEED/1000000))));
			HF_TaskYield();
		}

		printf("\n\nTest #2 - performance 1 to 1, high load");
		for(i=50; i<=1000; i+=50){
			printf("\nSending three %d byte payload messages... ", i);
			cycles = MemoryRead(COUNTER_REG);
			for(j=0; j<3; j++){
				s = HF_Send(HF_Core(1), 2, buf, i);
				if (s) printf("\nerror sending!");
			}
			cycles = MemoryRead(COUNTER_REG) - cycles;
			printf("%d cycles, %d Kbps", cycles, ((i * 5 * 8 * 1000) / (cycles / (CPU_SPEED/1000000))));
			HF_TaskYield();
		}

		printf("\n\nTest #3 - performance 1 to many");
		for(i=50; i<=1000; i+=50){
			printf("\nSending a %d byte payload message to all processors... ", i);
			cycles = MemoryRead(COUNTER_REG);
			for(j=1; j<6; j++){
				s = HF_Send(HF_Core(j), 2, buf, i);
				if (s) printf("\nerror sending!");
			}
			cycles = MemoryRead(COUNTER_REG) - cycles;
			printf("%d cycles, %d Kbps", cycles, ((i * 5 * 8 * 1000) / (cycles / (CPU_SPEED/1000000))));
			HF_TaskYield();
		}

		printf("\n\nTest #4 - performance 1 to many, high load");
		for(i=50; i<=1000; i+=50){
			printf("\nSending three %d byte payload messages to others... ", i);
			cycles = MemoryRead(COUNTER_REG);
			for(j=1; j<6; j++){
				for(k=0; k<3; k++){
					s = HF_Send(HF_Core(j), 2, buf, i);
					if (s) printf("\nerror sending!");
				}
			}
			cycles = MemoryRead(COUNTER_REG) - cycles;
			printf("%d cycles, %d Kbps", cycles, ((i * 5 * 5 * 8 * 1000) / (cycles / (CPU_SPEED/1000000))));
			HF_TaskYield();
		}

		printf("\n\nTest #6 - performance 1 to many, high load");

		printf("\nSending big payload messages to others... ", i);
		cycles = MemoryRead(COUNTER_REG);
		for(j=1; j<6; j++){
			for(k=0; k<3; k++){
				s = HF_Send(HF_Core(j), 2, buf, 1536);
				if (s) printf("\nerror sending!");
			}
		}
		cycles = MemoryRead(COUNTER_REG) - cycles;
		printf("%d cycles, %d Kbps", cycles, ((4096 * 5 * 5 * 8 * 1000) / (cycles / (CPU_SPEED/1000000))));
		HF_TaskYield();

		printf("\n\nTest #7 - performance 1 to many");
		for(i=50; i<=1500; i+=50){
			printf("\nSending a %d byte payload message to all processors... ", i);
			cycles = MemoryRead(COUNTER_REG);
			for(j=1; j<6; j++){
				s = HF_Send(HF_Core(j), 2, buf, i);
				if (s) printf("\nerror sending!");
			}
			cycles = MemoryRead(COUNTER_REG) - cycles;
			printf("%d cycles, %d Kbps", cycles, ((i * 5 * 8 * 1000) / (cycles / (CPU_SPEED/1000000))));
			HF_TaskYield();
		}

		printf("\n\nTest #8 - performance 1 to many");
		// we will fail sending the first message (if sw slots is 16). this is on purpose.
		for(i=1792; i>0; i-=128){
			printf("\nSending a %d byte payload message to all processors... ", i);
			cycles = MemoryRead(COUNTER_REG);
			for(j=1; j<6; j++){
				s = HF_Send(HF_Core(j), 2, buf, i);
				if (s) printf("\nerror sending!");
			}
			cycles = MemoryRead(COUNTER_REG) - cycles;
			printf("%d cycles, %d Kbps", cycles, ((i * 5 * 8 * 1000) / (cycles / (CPU_SPEED/1000000))));
			HF_TaskYield();
		}

		for(j=1; j<6; j++){
			s = HF_Send(HF_Core(j), 2, buf2, sizeof(buf2));
			if (s) printf("\nerror sending!");
		}

		sleep(10);
		HF_Panic(0);
	}
}

void dummy(void){
	for(;;){
	}
}

#else

void task_recv(void){
	uint16_t source_cpu, status;
	uint8_t source_id;
	uint16_t size;
	// messages from core 0 are big and we must have room to put them.
	uint8_t buf[2000];

	while(1){
		HF_Receive(&source_cpu, &source_id, buf, &size);
		printf("\nsize: %d loss: %d buf %d%% full", size, HF_PacketsLost(HF_CurrentTaskId()), (HF_PacketsQueued(HF_CurrentTaskId()) * 100) / HF_PACKET_SLOTS);
		printf("\nmsg: %s", buf);
		if (strcmp(buf, "test") == 0){
			sleep(10);
			HF_Panic(0);
		}
	}
}
#endif

void ApplicationMain(void){
#if CPU_ID == 0
//	HF_AddPeriodicTask(task_send, 20,19,20, "sender", 4096, 1, TASK_DEFAULT);
	HF_AddTask(dummy, "dummy", 2048, 1, TASK_DEFAULT);
	HF_AddTask(task_send, "sender", 4096, 1, TASK_DEFAULT);
#else
//	HF_AddPeriodicTask(task_recv, 20, 19, 20, "receiver", 4096, 1, TASK_DEFAULT);
	HF_AddTask(task_recv, "receiver", 4096, 1, TASK_DEFAULT);
#endif

	HF_Start();
}
