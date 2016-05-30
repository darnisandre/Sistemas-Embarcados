#include <prototypes.h>
#include "image.h"
#define num_cpu  9
#define block_width 32
#define block_area 1024 //32*32

semaphore sem[num_cpu-1];


uint8_t gausian(uint8_t buffer[7][7]){
	int32_t sum=0, mpixel;
	uint8_t x,y;

	int8_t kernel[7][7] =	{{1, 1, 2, 2, 2, 1, 1},
				{1, 3, 4, 5, 4, 3, 1},
				{2, 4, 7, 8, 7, 4, 2},
				{2, 5, 8, 10, 8, 5, 2},
				{2, 4, 7, 8, 7, 4, 2},
				{1, 3, 4, 5, 4, 3, 1},
				{1, 1, 2, 2, 2, 1, 1}};
	for(y=0;y<7;y++)
		for(x=0;x<7;x++)
			sum += ((int16_t)buffer[y][x] * (int16_t)kernel[y][x]);
	mpixel = (int32_t)(sum/171);

	return (uint8_t)mpixel;
}

void copyPart(uint8_t *buf, int part){
	int col = part % (width/block_width);
	int line = part / (width/block_width);
	int i= 0;
	printf("oi %d,%d,%d\n",part,col,line);
	for(;i<block_width; i++){
		int j = 0;
		for(;j<block_width; j++){
			int coluna = col*block_width + j;
			int linha = line*block_width +i;
			buf[i*block_width + j] = image[linha * width + coluna];
		}
	}
}

void putPart(uint8_t *buf, int part){
	int col = part % (width/block_width);
	int line = part / (width/block_width);
	int i= 0;
	
	for(;i<block_width; i++){
		int j = 0;
		for(;j<block_width; j++){
			int coluna = col*block_width + j;
			int linha = line*block_width +i;
			image[linha * width + coluna] = buf[i*block_width + j];
		}
	}
}


void master(void){
	uint8_t *buf = (uint8_t *) malloc(block_area);
	int i;

	uint32_t x,y,z;
	uint32_t time;
	printf("\n\nstart of processing!\n\n");

	time = MemoryRead(COUNTER_REG);

	for(i = 0; i < (width/block_width) * (height/block_width); i++){
		copyPart(buf,i);
		HF_Send(HF_Core( (i%(num_cpu -1)) + 1), 2, buf, block_area );
	}

	for(;i<num_cpu-1;i++){
		HF_SemWait(&sem[i]);
	}

	time = MemoryRead(COUNTER_REG) - time;

	printf("done in %d clock cycles.\n\n", time);

	printf("\n\nint32_t width = %d, height = %d;\n", width, height);
	printf("uint8_t image[] = {\n");
	for(y=0;y<height;y++){
		for(x=0;x<width;x++){
			printf("0x%x", image[y*width+x]);
			if ((y < height-1) || (x < width-1)) printf(", ");
			if ((++z % 16) == 0) printf("\n");
		}
	}
	printf("};\n");

	while(1);
}

void masterReceiver(void){
	uint16_t source_cpu, status;
	uint8_t source_id;
	uint8_t *buf = (uint8_t *) malloc(block_area);
	int i =0;
	for(;i<2;i++){
		HF_Receive(&source_cpu, &source_id, buf,block_area);
		putPart( buf, (source_cpu-1) + i * (num_cpu -1) );
	}

	HF_SemPost(&sem[source_cpu-1]);

	while(1);
}

void do_gausian(uint8_t *img){
	int32_t x,y,u,v;
	uint8_t image_buffer[7][7];
	
	for(y=0;y<block_width;y++){
		if (y > 2 || y < block_width-2){
			for(x=0;x<block_width;x++){
				if (x > 2 || x < block_width-2){
					for(v=0;v<7;v++)
						for(u=0;u<7;u++)
							image_buffer[v][u] = img[(((y+v-3)*block_width)+(x+u-3))];

					img[((y*block_width)+x)] = gausian(image_buffer);
				}else{
					img[((y*block_width)+x)] = img[((y*block_width)+x)];
				}
			}
		}else{
			img[((y*block_width)+x)] = img[((y*block_width)+x)];
		}
	}
}

void slave(void){
	uint16_t source_cpu, status;
	uint8_t source_id;
	uint8_t *buf = (uint8_t *) malloc(block_area);
	while(1){
		HF_Receive(&source_cpu, &source_id, buf,block_area);
		do_gausian(buf);
		HF_Send(HF_Core(0), HF_CurrentCpuId() + 2, buf, block_area );
	}
}

void ApplicationMain(void) {
#if CPU_ID == 0
	int i=0;
	for(;i<num_cpu-1;i++){
		HF_SemInit(&sem[i],0);
	}


	HF_AddTask(master, "master", 2048, 10, 0);
	HF_AddTask(masterReceiver, "masterReceiver1", 2048, 10, 0);
	HF_AddTask(masterReceiver, "masterReceiver2", 2048, 10, 0);
	HF_AddTask(masterReceiver, "masterReceiver3", 2048, 10, 0);
	HF_AddTask(masterReceiver, "masterReceiver4", 2048, 10, 0);
	HF_AddTask(masterReceiver, "masterReceiver5", 2048, 10, 0);
	HF_AddTask(masterReceiver, "masterReceiver6", 2048, 10, 0);
	HF_AddTask(masterReceiver, "masterReceiver7", 2048, 10, 0);
	HF_AddTask(masterReceiver, "masterReceiver8", 2048, 10, 0);
	
#endif
#if CPU_ID != 0
	HF_AddTask(slave, "slave", 2048, 10, 0);
#endif

	HF_Start();
}

