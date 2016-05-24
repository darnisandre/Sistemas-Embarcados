#include <prototypes.h>
#define N 20

semaphore empty, full;
semaphore m;

int32_t in=0, out=0, buffer[N];

void producer(void){
	int32_t item;

	while(1){
		item=random();
		HF_SemWait(&empty);
		HF_SemWait(&m);
			buffer[in]=item;
			printf("\nproducer %d putting on position %d", HF_CurrentTaskId(), in);
			in=(in+1)%N;
		HF_SemPost(&m);
		HF_SemPost(&full);
	}
}

void consumer(void){
	int32_t item;

	while(1){
		HF_SemWait(&full);
		HF_SemWait(&m);
			item=buffer[out];
			printf("\nconsumer %d taking from position %d", HF_CurrentTaskId(), out);
			out=(out+1)%N;
		HF_SemPost(&m);
		HF_SemPost(&empty);
		item=0;
	}
}

void ApplicationMain(void){
	HF_SemInit(&empty,N);
	HF_SemInit(&full,0);
	HF_SemInit(&m,1);

	HF_AddTask(producer, "Producer A", 2048, 50, 0);
	HF_AddTask(producer, "Producer B", 2048, 50, 0);
	HF_AddTask(producer, "Producer C", 2048, 50, 0);
	HF_AddTask(producer, "Producer D", 2048, 50, 0);
	HF_AddTask(consumer, "Consumer A", 2048, 50, 0);
	HF_AddTask(consumer, "Consumer B", 2048, 50, 0);
	HF_AddTask(consumer, "Consumer C", 2048, 50, 0);
	HF_AddTask(consumer, "Consumer D", 2048, 50, 0);

	HF_Start();
}

