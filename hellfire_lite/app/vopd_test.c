// VOPD application model

#include <prototypes.h>

void task3(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];
	uint32_t time;

	while(1){
//		time = MemoryRead(COUNTER_REG);
		for(i=0; i<458*16; i++);				// 5500 cc -> proc time = cycles / 12
		HF_MsgSend(4, "three->demux", 100);
		HF_MsgSend(17, "three->demux", 100);
		HF_MsgRecv(&task, buf, &size);
//		time = MemoryRead(COUNTER_REG) - time;
//		printf("\ntask %d -> task %d size %d msg %s proc time %d", HF_CurrentTaskId(), task, size, buf, time);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
	}
}

void task4(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(5, "four->var length decoder*", 70);
	}
}

void task5(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(6, "five->run length decoder", 362);
	}
}

void task6(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(7, "six->inverse scan", 362);
	}
}

void task7(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(8, "seven->ac/dc prediction", 362);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
	}
}

void task8(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(9, "eight->iquant", 357);
		HF_MsgSend(19, "eight->iquant", 49);
	}
}

void task9(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(10, "nine->idct", 353);
	}
}

void task10(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(11, "ten->up samp", 300);
	}
}

void task11(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(12, "eleven->VOP reconstr", 313);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
	}
}

void task12(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(13, "twelve->padding", 313);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
	}
}

void task13(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(11, "thirteen->VOP memory", 500);
		HF_MsgSend(12, "thirteen->VOP memory", 94);
		HF_MsgSend(3, "end of algorithm!", 50);
	}
}

void task14(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(15, "fourteen->up samp", 16);
	}
}

void task15(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(9, "fifteen->ref memory", 16);
		HF_MsgSend(12, "fifteen->ref memory", 16);
		HF_MsgSend(16, "fifteen->ref memory", 16);
	}
}

void task16(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(17, "sixteen->downsample and context calculation", 157);
	}
}

void task17(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(18, "seventeen->arithmetic decoder*", 16);
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
	}
}

void task18(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(14, "eighteen->memory", 16);
		HF_MsgSend(16, "eighteen->memory", 16);
	}
}

void task19(void){
	uint16_t size, i;
	uint8_t task;
	uint8_t buf[500];

	while(1){
		HF_MsgRecv(&task, buf, &size);
		printf("\ntask %d -> task %d size %d msg %s", HF_CurrentTaskId(), task, size, buf);
		for(i=0; i<458*16; i++);
		HF_MsgSend(7, "nineteen->stripe memory", 27);
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

	HF_AddTask(task3, "task graph", 2048, 40, 0);
	HF_AddTask(task4, "task graph", 2048, 40, 0);
	HF_AddTask(task5, "task graph", 2048, 40, 0);
	HF_AddTask(task6, "task graph", 2048, 40, 0);
	HF_AddTask(task7, "task graph", 2048, 40, 0);
	HF_AddTask(task8, "task graph", 2048, 40, 0);
	HF_AddTask(task9, "task graph", 2048, 40, 0);
	HF_AddTask(task10, "task graph", 2048, 40, 0);
	HF_AddTask(task11, "task graph", 2048, 40, 0);
	HF_AddTask(task12, "task graph", 2048, 40, 0);
	HF_AddTask(task13, "task graph", 2048, 40, 0);
	HF_AddTask(task14, "task graph", 2048, 40, 0);
	HF_AddTask(task15, "task graph", 2048, 40, 0);
	HF_AddTask(task16, "task graph", 2048, 40, 0);
	HF_AddTask(task17, "task graph", 2048, 40, 0);
	HF_AddTask(task18, "task graph", 2048, 40, 0);
	HF_AddTask(task19, "task graph", 2048, 40, 0);

	HF_Start();
}

