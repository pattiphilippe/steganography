#include <stdlib.h>
#include "gif.h"



enum gif_section read_gif_section(FILE * source){
	unsigned char buffer;
	bool again;
	do {
		again = false;
		fread(&buffer, sizeof(char), 1, source);
		printf("bite of section type read : 0x%0x\n", buffer);
		switch(buffer){
			case 0X2C:
				return image_descr;
			case 0X21:
				again = true;
				break;
			case 0Xf9:
				return graphics;
			case 0X01:
				return plain_text;
			case 0Xff:
				return application;
			case 0Xfe:
				return comment;
			case 0X3B:
		        return trailer;
		}
	} while (again);
	return -1;			
}


void passSection(FILE * source, enum gif_section section){
	switch(section){
		case 0:
		case 1:
		case 2:
		case 3:
			passDataSubBlocks(source);
			break;
		default:
			perror("gif.c::passSection : Unknown Section");
			exit(1);
	}
}


void passDataSubBlocks(const FILE * source){
	unsigned char size;
	fread(&size, sizeof(size), 1, source);
	while(size){
  		fseek(source, size, SEEK_CUR);
		fread(&size, sizeof(char), 1, source);
	}
}







