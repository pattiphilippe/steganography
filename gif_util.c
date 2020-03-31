#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include "gif_util.h"

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