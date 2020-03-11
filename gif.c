#include <stdlib.h>
#include "gif.h"



enum gif_section read_gif_section(FILE * source){
	unsigned char buffer;
	bool again;
	do {
		again = false;
		fread(&buffer, sizeof(char), 1, source);
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


int passSection(FILE * source, enum gif_section section){
	printf("in pass Section\n");
	switch(section){
		case 0:
		case 1:
		case 2:
		case 3:
			passDataSubBlocks(source);
			printf("back in pass Section\n");
			break;
		default:
			return -1;
	}
	return 0;
}


int passDataSubBlocks(FILE * source){
	unsigned char buffer, size;
	fread(&buffer, sizeof(char), 1, source);
	while(buffer){
		printf("in while with buffer %d\n", buffer);
		size = buffer;
		printf("size = %d\n\n", size);
		fread(&buffer, sizeof(char), size, source);
		fread(&buffer, sizeof(char), 1, source);
	}
	printf("passDataSubBlocks out of while\n");
	return 0;
}







