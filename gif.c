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
			default:
				exit(EXIT_FAILURE); 
		}
	} while (again);
			
}
