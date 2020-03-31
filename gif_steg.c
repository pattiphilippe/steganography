#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "gif_steg.h"


int getMaxLCT(const char *source)
{
	FILE *gif_src = fopen(source, "r");

	if (gif_src == NULL)
	{
		perror(strcat("gif.c::getMaxLCT : Error opening src file ", source));
		exit(1);
	}
	passHeaderLSDGCT(gif_src);

	int maxLCT = 0;
	gif_section_t section = read_gif_section(gif_src);
	while (section != trailer)
	{
		printf("in while section with section : %d, at ftell : %ld\n\n", section, ftell(gif_src));
		switch (section)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			passDataSubBlocks(gif_src);
			break;
		case 4:
			maxLCT++;
			passImageDescrBlock(gif_src);
			break;
		default:
			errno = 22;
			perror("Section is yet unknown! GIF structure is bad!\n");
			exit(1);
		}
		section = read_gif_section(gif_src);
	}
	return maxLCT;
}


void writeGifWithLCT(const char* source, const char* dest){

}

