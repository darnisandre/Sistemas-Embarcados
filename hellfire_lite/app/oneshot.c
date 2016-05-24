#include <prototypes.h>

void task1(void){
	while(1){
		printf("\nTask 1: I'll die in one system tick");
	}
}

void ApplicationMain(void){
	HF_AddTask(task1, "task", 2048, 50, TASK_ONESHOT);

	HF_Start();
}

