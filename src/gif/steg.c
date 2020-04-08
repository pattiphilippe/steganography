#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "steg.h"

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
	gif_section_t section = read_gif_section(gif_src, NULL, false);
	while (section != trailer)
	{
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
		section = read_gif_section(gif_src, NULL, false);
	}

	fclose(gif_src);
	return maxLCT;
}

void writeGifWithLCT(const char *source, const char *dest)
{
	FILE *gif_src = fopen(source, "r");
	if (gif_src == NULL)
	{
		perror(strcat("gif.c::writeGifWithLCT : Error opening src file ", source));
		exit(1);
	}
	FILE *gif_dest = fopen(dest, "w");
	if (gif_dest == NULL)
	{
		perror(strcat("gif.c::writeGifWithLCT : Error opening dest file ", dest));
		exit(1);
	}

	int sizeGCT = 0;
	long posGCT = 0;
	copyHeaderLsdGct(gif_src, gif_dest, &sizeGCT, &posGCT);

	gif_section_t section = read_gif_section(gif_src, gif_dest, true);
	while (section != trailer)
	{
		switch (section)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			copyDataSubBlocks(gif_src, gif_dest);
			break;
		case 4:
			copyImageDescrBlockWithLCT(gif_src, gif_dest, sizeGCT, posGCT);
			break;
		default:
			errno = 22;
			perror("Section is yet unknown! GIF structure is bad!\n");
			exit(1);
		}
		section = read_gif_section(gif_src, gif_dest, true);
	}

	fclose(gif_src);
	fclose(gif_dest);
}
