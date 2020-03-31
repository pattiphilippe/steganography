#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "gif.h"

bool hasColorTable(const unsigned char *packed_field)
{
	return *packed_field & (1 << 7);
}
unsigned sizeOfColorTable(const unsigned char *packed_field)
{
	unsigned char color_resolution = (*packed_field & 0x70) >> 4;
	return 3 * pow(2.0, (color_resolution + 1));
}

gif_section_t read_gif_section(FILE *source)
{
	unsigned char buffer;
	bool again;
	do
	{
		again = false;
		fread(&buffer, sizeof(char), 1, source);
		printf("bite of section type read : 0x%02x\n", buffer);
		switch (buffer)
		{
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
	errno = 22;
	perror("unknown section");
	exit(0);
}

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

void passDataSubBlocks(FILE *source)
{
	unsigned char size;
	fread(&size, sizeof(size), 1, source);
	while (size)
	{
		fseek(source, size, SEEK_CUR);
		fread(&size, sizeof(char), 1, source);
	}
}

void passHeaderLSDGCT(FILE *source){
	header_lsd_t header_lsd;
	fread(&header_lsd, sizeof(char), sizeof(header_lsd_t), source);
	bool hasGCT = hasColorTable(&(header_lsd.packed_field));
	if (hasGCT)
	{
		unsigned sizeGCT = sizeOfColorTable(&(header_lsd.packed_field));
		fseek(source, sizeGCT, SEEK_CUR);
	}
}

void passImageDescrBlock(FILE *source)
{
	image_descr_t image_descr;
	fread(&image_descr, sizeof(char), sizeof(image_descr), source);
	if (hasColorTable(&(image_descr.packed_field)))
	{
		unsigned sizeLCT = sizeOfColorTable(&(image_descr.packed_field));
		fseek(source, sizeLCT, SEEK_CUR);
	}
	fseek(source, 1, SEEK_CUR); // in image data, passing LZW minimum code size byte
	passDataSubBlocks(source);
}
