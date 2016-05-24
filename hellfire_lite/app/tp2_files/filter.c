#include <prototypes.h>
#include "image.h"
#define num_cpu  9
#define block_width 32
union msg {
	uint8_t buf[block_width][block_width];
};


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

void do_gausian(uint8_t *img){
	int32_t x,y,u,v;
	uint8_t image_buffer[7][7];
	
	for(y=0;y<height;y++){
		if (y > 2 || y < height-2){
			for(x=0;x<width;x++){
				if (x > 2 || x < width-2){
					for(v=0;v<7;v++)
						for(u=0;u<7;u++)
							image_buffer[v][u] = image[(((y+v-3)*width)+(x+u-3))];

					img[((y*width)+x)] = gausian(image_buffer);
				}else{
					img[((y*width)+x)] = image[((y*width)+x)];
				}
			}
		}else{
			img[((y*width)+x)] = image[((y*width)+x)];
		}
	}
}

void task(void){
	uint32_t x,y,z;
	uint8_t *img;
	uint32_t time;
	
	while(1) {
		img = (uint8_t *) malloc(height * width);
		if (img == NULL){
			printf("\nmalloc() failed!\n");
			for(;;);
		}

		printf("\n\nstart of processing!\n\n");

		time = MemoryRead(COUNTER_REG);

		do_gausian(img);

		time = MemoryRead(COUNTER_REG) - time;

		printf("done in %d clock cycles.\n\n", time);

		printf("\n\nint32_t width = %d, height = %d;\n", width, height);
		printf("uint8_t image[] = {\n");
		for(y=0;y<height;y++){
			for(x=0;x<width;x++){
				printf("0x%x", img[y*width+x]);
				if ((y < height-1) || (x < width-1)) printf(", ");
				if ((++z % 16) == 0) printf("\n");
			}
		}
		printf("};\n");

		free(img);

		printf("\n\nend of processing!\n");
		HF_Panic(0);
	}
		
}

void copyPart(msg *message, int part){
	int col = part % width/block_width;
	int line = part / (width/block_width);
	int i= 0;
	for(;i<block_width; i++){
		int j = 0;
		for(;block_width; j++){
			int coluna = col*block_width + j;
			int linha = line*block_width +i;
			message->buf[i][j] = image[linha * width + coluna];
		}
	}
}

void master(void){


	union msg message;
	for(i = 1; i < width/block_width * height/block_width; i++){
		copyPart(&message,i-1)
		s = HF_Send(HF_Core(i), 2, message.buf, sizeof(union msg));
		if (s) printf("\nerror sending!");
	}

}

void slave(void){}
void ApplicationMain(void) {
#if CPU_ID == 4
	HF_AddTask(task, "filter", 2048, 10, 0);
#endif

	HF_Start();
}

