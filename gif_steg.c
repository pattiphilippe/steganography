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
	passHeaderLsdGct(gif_src);

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

	fclose(gif_src);
	return maxLCT;
}

void writeGifWithLCT(const char *source, const char *dest)
{
	printf("in writeGifWithLCT\n");

	FILE *gif_src = fopen(source, "r");
	if (gif_src == NULL)
	{
		//perror(strcat("gif.c::writeGifWithLCT : Error opening src file ", source));
		printf("gif.c::writeGifWithLCT : Error opening src file %s\n", source);
		exit(1);
	} else {
		printf("opened source file : %s\n", source);
	}
	FILE *gif_dest = fopen(dest, "w");
	if (gif_dest == NULL)
	{
		// perror(strcat("gif.c::writeGifWithLCT : Error opening dest file ", dest));
		printf("gif.c::writeGifWithLCT : Error opening dest file %s\n", dest);
		exit(1);
	} else {
		printf("opened dest file : %s\n", dest);
	}

	int sizeGCT = 0;
	long posGCT = 0;
	printf("writeGifWithLCT\n");
	copyHeaderLsdGct(gif_src, gif_dest, true, &sizeGCT, &posGCT);

	printf("writeGifWithLCT after copy Header...\n");
	printf("sizeGCT = %d, posGCT = %ld\n", sizeGCT, posGCT);
}
