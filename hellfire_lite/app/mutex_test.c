#include <prototypes.h>

uint32_t shared;
mutex m;

void task(void){
	int8_t buf[20];
	uint32_t c = 0;

	while(1){
		HF_EnterRegion(&m);
		shared = random();
		printf("\nTask %d on critical region.. shared: %d c: %d", HF_CurrentTaskId(), shared, c++);
		HF_LeaveRegion(&m);
	}
}

void ApplicationMain(void){
	HF_AddTask(task, "Task A", 2048, 10, 0);
	HF_AddTask(task, "Task B", 2048, 20, 0);
	HF_AddTask(task, "Task C", 2048, 30, 0);
	HF_AddTask(task, "Task D", 2048, 40, 0);
	HF_AddTask(task, "Task E", 2048, 50, 0);
	HF_AddTask(task, "Task F", 2048, 60, 0);
	HF_AddTask(task, "Task E", 2048, 200, 0);

	HF_Start();
}

