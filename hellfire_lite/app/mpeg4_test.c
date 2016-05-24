// MPEG4 application model

#include <prototypes.h>

void task3(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];
	uint32_t time;

	while(1){
		time = MemoryRead(COUNTER_REG);
		for(i=0; i<458*16; i++);				// 5768 cc -> proc time = cycles / 12
		HF_MsgSend(4, "three->SRAM New Frame", 384);
		HF_MsgRecv(&task, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\ntask %d -> task %d size %d msg %s proc time %d", HF_CurrentTaskId(), task, size, buf, time);
	}
}

void task4(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(5, "four->Input Control", 384);
		HF_MsgSend(6, "four->Input Control", 256);
	}
}

void task5(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(9, "five->FIFO Current MBL", 384);
	}
}

void task6(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(7, "six->FIFO New MBL", 128);
	}
}

void task7(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(8, "seven->ME", 12);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
	}
}

void task8(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(9, "eight->FIFO MV", 12);
	}
}

void task9(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(10, "nine->MC", 432);
		HF_MsgSend(11, "nine->MC", 384);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		HF_MsgSend(10, "nine->MC", 432);
	}
}

void task10(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(12, "ten->Error Block", 432);
	}
}

void task11(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(14, "eleven->Comp Block", 384);
	}
}

void task12(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(13, "twelve->Texture Coding", 432);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(19, "twelve->Texture Coding", 576);
	}
}

void task13(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(14, "thirteen->Texture Block", 432);
	}
}

void task14(void){
	uint16_t cpu, size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(15, "fourteen->Texture Update", 256);
	}
}

void task15(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(16, "fifteen->SRAM RecFrame", 384);
	}
}

void task16(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(17, "sixteen->Copy Controller", 768);
		HF_MsgSend(18, "sixteen->Copy Controller", 768);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
	}
}

void task17(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(7, "seventeen->Search Area", 267);
	}
}

void task18(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(16, "eighteen->YUV Buffer", 384);
		HF_MsgSend(9, "eighteen->YUV Buffer", 384);
	}
}

void task19(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(20, "nineteen->Quantised MBL", 576);
	}
}

void task20(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[768];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(3, "twenty->VLC", 58);
	}
}

void dummy_comm_driver(void){
	for(;;);
}

void dummy_migration_mgr(void){
	for(;;);
}

void ApplicationMain(void){
	HF_AddTask(dummy_comm_driver, "task graph", 1024, 0, 0);
	HF_AddTask(dummy_migration_mgr, "task graph", 1024, 0, 0);

	HF_AddTask(task3, "task graph", 2568, 50, 0);
	HF_AddTask(task4, "task graph", 2568, 50, 0);
	HF_AddTask(task5, "task graph", 2568, 50, 0);
	HF_AddTask(task6, "task graph", 2568, 50, 0);
	HF_AddTask(task7, "task graph", 2568, 50, 0);
	HF_AddTask(task8, "task graph", 2568, 50, 0);
	HF_AddTask(task9, "task graph", 2568, 50, 0);
	HF_AddTask(task10, "task graph", 2568, 50, 0);
	HF_AddTask(task11, "task graph", 2568, 50, 0);
	HF_AddTask(task12, "task graph", 2568, 50, 0);
	HF_AddTask(task13, "task graph", 2568, 50, 0);
	HF_AddTask(task14, "task graph", 2568, 50, 0);
	HF_AddTask(task15, "task graph", 2568, 50, 0);
	HF_AddTask(task16, "task graph", 2568, 50, 0);
	HF_AddTask(task17, "task graph", 2568, 50, 0);
	HF_AddTask(task18, "task graph", 2568, 50, 0);
	HF_AddTask(task19, "task graph", 2568, 50, 0);
	HF_AddTask(task20, "task graph", 2568, 50, 0);

	HF_Start();
}

