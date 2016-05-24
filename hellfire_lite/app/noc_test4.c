#include <prototypes.h>

void task_send(void){
	uint32_t i, s;

	union msg {
		uint8_t buf[100];
		int32_t numbers[5];
	};

	union msg message;

	message.numbers[0] = 100;
	message.numbers[1] = 150;
	message.numbers[2] = 200;
	message.numbers[3] = -1000;
	message.numbers[4] = -1500;

	while(1){
		for(i = 1; i < 6; i++){
			s = HF_Send(HF_Core(i), 2, message.buf, sizeof(union msg));
			if (s) printf("\nerror sending!");
		}

		for(i = 1; i < 6; i++){
			s = HF_Send(HF_Core(i), 2, "test", 5);
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

	union msg {
		uint8_t buf[100];
		int32_t numbers[5];
	};

	union msg message;

	while(1){
		HF_Receive(&source_cpu, &source_id, message.buf, &size);
		printf("\nrecv size: %d loss: %d", size, HF_PacketsLost(HF_CurrentTaskId()));
		if (strcmp(message.buf, "test") == 0){
			sleep(10);
			HF_Panic(0);
		}
		printf("\nnumbers: %d %d %d %d %d", message.numbers[0], message.numbers[1], message.numbers[2], message.numbers[3], message.numbers[4]);
	}
}

void ApplicationMain(void){
#if CPU_ID == 0
	HF_AddTask(task_send, "sender", 2048, 1, TASK_DEFAULT);
#else
	HF_AddTask(task_recv, "receiver", 2048, 1, TASK_DEFAULT);
#endif

	HF_Start();
}

