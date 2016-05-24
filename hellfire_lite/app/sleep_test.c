#include <prototypes.h>

void task1(void){
	uint32_t a=0;

	while(1){
		printf("\nTask 1: %d", a);
		a++;		
		sleep(100);			// wait for 100ms
	}
}

void task2(void){
	uint32_t a=0;

	while(1){
		printf("\nTask 2: %d", a);
		a++;
		usleep(110000);			// wait for 110ms
	}
}

void task3(void){
	uint32_t a=0;

	while(1){
		printf("\nTask 3: %d seconds have passed...", a);
		a++;		
		sleep(1000);			// wait for 1000ms
	}
	
}

void ApplicationMain(void){
	HF_AddTask(task1, "tarefa 1", 2048, 20, 0);
	HF_AddTask(task2, "tarefa 2", 2048, 20, 0);
	HF_AddTask(task3, "tarefa 3", 2048, 20, 0);

	HF_Start();
}

