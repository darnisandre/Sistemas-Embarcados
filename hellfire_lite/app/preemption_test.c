#include <prototypes.h>

void task1(void){
	while(1){
		printf("\nThis text will be messed. A task can be interrupted at any moment");
		printf("\nCritical sections can be used to avoid this");
	}
}

void task2(void){
	int32_t i = 0;

	while(1){
		printf("\nI am another task, and I just count => %d", i++);
	}
}

void task3(void){
	while(1){
		printf("\nHello World! The last context switch time was %d cycles and I executed for %d cycles last time", HF_LastContextSwitchTime(), HF_LastTickTime(HF_CurrentTaskId()));
	}
}

void ApplicationMain(void){
	HF_AddTask(task1, "one", 800, 30, 0);
	HF_AddTask(task2, "two", 800, 30, 0);
	HF_AddTask(task3, "three", 800, 30, 0);
	HF_Start();
}

