#include <stdio.h>
#include <malloc.h>
#include <stdint.h>

uint8_t *buffer_0, *buffer_1, *buffer_2;

int32_t load_bmp(int32_t *width, int32_t *height, int8_t bmp[]){
	struct bmpinfoheader{
		uint16_t fileid;
		uint32_t filesize;
		uint16_t reserved1;
		uint16_t reserved2;
		uint32_t imgoffset;
	} bmpheader;
	struct imginfoheader{
		uint32_t headersize;
		int32_t imgwidth;
		int32_t imgheight;
		uint16_t numplanes;
		uint16_t pixeldepth;
		uint32_t compression;
		uint32_t bitmapsize;
		int32_t hresolution;
		int32_t vresolution;
		uint32_t usedcolors;
		uint32_t significantcolors;
	} imgheader;

	FILE *fptr;
	int32_t x,y;
	uint32_t padbytes=0;
	uint8_t ch;

	if((fptr=fopen(bmp,"rb"))==NULL){
		printf("error: file not found.");
		return -1;
	}else{
		fread(&bmpheader.fileid,2,1,fptr);
		fread(&bmpheader.filesize,4,1,fptr);
		fread(&bmpheader.reserved1,2,1,fptr);
		fread(&bmpheader.reserved2,2,1,fptr);
		fread(&bmpheader.imgoffset,4,1,fptr);
		fread(&imgheader,sizeof(imgheader),1,fptr);

		if (	(bmpheader.fileid != 19778) ||
			(bmpheader.imgoffset != 54) ||
			(imgheader.headersize != 40) ||
			(imgheader.numplanes != 1) ||
			(imgheader.pixeldepth != 24) ||
			(imgheader.compression != 0)
		){
			printf("error: Invalid bitmap image.");
			return -1;
		};

		padbytes = (imgheader.imgwidth*3)%4;
		if(padbytes!=0)
			padbytes = 4-padbytes;
		buffer_0 = (uint8_t *) malloc(imgheader.imgheight*imgheader.imgwidth*sizeof(uint8_t));
		buffer_1 = (uint8_t *) malloc(imgheader.imgheight*imgheader.imgwidth*sizeof(uint8_t));
		buffer_2 = (uint8_t *) malloc(imgheader.imgheight*imgheader.imgwidth*sizeof(uint8_t));
		if (buffer_0 == NULL || buffer_1 == NULL || buffer_2 == NULL){
			printf("out of memory.");
			return -1;
		}
		for(x=imgheader.imgheight-1;x>-1;x--){
			for(y=0;y<imgheader.imgwidth;y++){
				buffer_2[(x*imgheader.imgwidth+y)] = getc(fptr);
				buffer_1[(x*imgheader.imgwidth+y)] = getc(fptr);
				buffer_0[(x*imgheader.imgwidth+y)] = getc(fptr);
			}
			for(y=0;y<padbytes;y++)
				getc(fptr);
		}
		*width = imgheader.imgwidth;
		*height = imgheader.imgheight;
		fclose(fptr);
	}
}

int32_t main(void){
	int32_t width, height;
	int32_t x,y,z=0;

	load_bmp(&width, &height, "lena_gray.bmp");

	printf("int32_t width = %d, height = %d;\n", width, height);

	printf("uint8_t image[] = {\n");
	for(y=0;y<height;y++){
		for(x=0;x<width;x++){
			printf("0x%02x", buffer_0[y*width+x]);
			if ((y < height-1) || (x < width-1)) printf(", ");
			if ((++z % 16) == 0) printf("\n");
		}
	}
	printf("};\n");
	
	free(buffer_0);
	free(buffer_1);
	free(buffer_2);

	return 0;
}

