#include <prototypes.h>

void task(void){
	int8_t buf[20];
	uint8_t *addr, *addr2;
	uint32_t i,j;

	while(1){
		printf("\nfree memory: %d bytes", HF_FreeMemory());
		printf("\nmalloc(): allocating 4096 bytes on heap...");
		addr = (uint8_t *)malloc(4096);
		if (addr){
			printf("\nfree memory: %d bytes", HF_FreeMemory());
			printf("\ndeallocating the same 4096 bytes from heap...");
			free(addr);
		}else{
			printf("\nTest failed.");
			for(;;);
		}
		printf("\nfree memory: %d bytes", HF_FreeMemory());
		printf("\ncalloc(): allocating 4096 bytes on heap...");
		addr = (uint8_t *)calloc(4096, 1);
		if (addr){
			printf("\nfree memory: %d bytes", HF_FreeMemory());
			printf("\ndeallocating the same 4096 bytes from heap...");
			free(addr);
		}else{
			printf("\nTest failed.");
			for(;;);
		}
		printf("\nfree memory: %d bytes", HF_FreeMemory());
		printf("\nmalloc(): allocating 1024 bytes on heap...");
		addr = (uint8_t *)malloc(1024);
		if (addr){
			printf("\nfree memory: %d bytes", HF_FreeMemory());
			printf("\nrealloc() reallocating to 4096 bytes the old buffer and creating a new one...");
			addr2 = (uint8_t *)realloc(addr, 4096);
			free(addr);
		}else{
			printf("\nTest failed.");
			for(;;);
		}
		if (addr2){
			printf("\nfree memory: %d bytes", HF_FreeMemory());
			printf("\ndeallocating the new buffer...");
			free(addr2);
		}else{
			printf("\nTest failed.");
			for(;;);
		}
		printf("\nfree memory: %d bytes", HF_FreeMemory());
		printf("\nmalloc(): allocating 25000 bytes on heap...");
		addr = (uint8_t *)malloc(25000);
		if (addr){
			printf("\nfree memory: %d bytes", HF_FreeMemory());
			printf("\ndeallocating the same 25000 bytes from heap...");
			free(addr);
		}else{
			printf("\nTest failed.");
			for(;;);
		}

		printf("\nfree memory: %d bytes", HF_FreeMemory());

		printf("\nmalloc(): allocating 150MB!!! this will only work on x86");
		addr = (uint8_t *)malloc(157286400);
		if (addr){
			printf("\nfree memory: %d bytes", HF_FreeMemory());
			free(addr);
		}else{
			printf("\nTest failed.");
		}

		HF_Panic(0);
	}
}

void ApplicationMain(void){
	HF_AddTask(task, "Task A", 2048, 50, 0);

	HF_Start();
}

