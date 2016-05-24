#include <prototypes.h>

void task_a(void){
	uint8_t buf[100], buf2[10];
	uint32_t i=0;

	while(1){
		strcpy(buf, "\nhello from task 0 ==> ");
		itoa(i++, buf2, 10);
		strcat(buf, buf2);
		HF_MsgSend(2, buf, sizeof(buf));
		HF_MsgSend(3, "\nhello world!", 15);
	}
}

void task_b(void){
	uint8_t buf[100];
	uint8_t source_id;
	uint16_t size;

	while(1){
		HF_MsgRecv(&source_id, buf, &size);
		printf("%s", buf);
	}
}

void ApplicationMain(void){
	HF_AddTask(task_a, "task a", 2048, 50, 0);
	HF_AddTask(task_b, "task b", 2048, 50, 0);
	HF_AddTask(task_b, "task c", 2048, 50, 0);

	HF_Start();
}

