#include <prototypes.h>

void task(void){
	int32_t i, val,c=0;
	uint8_t forked=0;

	while(1){
		printf("\nTask %d, c == %d", HF_CurrentTaskId(), c);
		c++;
		if ((forked == 0) && (c > 50)){
			forked = 1;
			val = HF_Fork();
			
			if (val > -1){
				printf("\nHF_Fork() succeded!");
			}else{
				printf("\nHF_Fork() failed!");
				for(;;);
			}
		}
	}
}

void ApplicationMain(void){
	HF_AddTask(task, "fork", 2048, 10, 0);

	HF_Start();
}

