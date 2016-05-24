#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include "filter_image.h"

int32_t save_bmp(char bmp[]){
	struct bmpinfoheader{
		uint16_t fileid;
		uint32_t filesize;
		uint16_t reserved1;
		uint16_t reserved2;
		uint32_t moffset;
	} bmpheader;
	struct minfoheader{
		uint32_t headersize;
		int32_t mwidth;
		int32_t mheight;
		uint16_t numplanes;
		uint16_t pixeldepth;
		uint32_t compression;
		uint32_t bitmapsize;
		int32_t hresolution;
		int32_t vresolution;
		uint32_t usedcolors;
		uint32_t significantcolors;
	} mheader;

	FILE *fptr;
	int32_t x,y,c;
	uint32_t padbytes=0;
	uint8_t ch;

	bmpheader.fileid = 19778;
	bmpheader.filesize = width * height + 54;
	bmpheader.reserved1 = 0;
	bmpheader.reserved2 = 0;
	bmpheader.moffset = 54;
	mheader.headersize = 40;
	mheader.mwidth = width;
	mheader.mheight = height;
	mheader.numplanes = 1;
	mheader.pixeldepth = 24;
	mheader.compression = 0;
	mheader.bitmapsize = width * height;
	mheader.hresolution = 0;
	mheader.vresolution = 0;
	mheader.usedcolors = 0;
	mheader.significantcolors = 0;

	if((fptr=fopen(bmp,"wb"))==NULL){
		printf("\nCould not write to file.");
		return 1;
	}else{
		fwrite(&bmpheader.fileid,2,1,fptr);
		fwrite(&bmpheader.filesize,4,1,fptr);
		fwrite(&bmpheader.reserved1,2,1,fptr);
		fwrite(&bmpheader.reserved2,2,1,fptr);
		fwrite(&bmpheader.moffset,4,1,fptr);
		fwrite(&mheader,sizeof(mheader),1,fptr);
		c=0;

		padbytes = (mheader.mwidth*3)%4;
		if(padbytes!=0)
			padbytes = 4-padbytes;
		for(x=mheader.mheight-1;x>-1;x--){
			for(y=0;y<mheader.mwidth;y++){
				putc(image[(x*mheader.mwidth+y)],fptr);
				putc(image[(x*mheader.mwidth+y)],fptr);
				putc(image[(x*mheader.mwidth+y)],fptr);
			}
			for(y=0;y<padbytes;y++)
				putc(0,fptr);
		}
		fclose(fptr);
	}
	return 0;
}


int32_t main(){
	save_bmp("output.bmp");

	return 0;
	
}
