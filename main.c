#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#include "gif.h"

void printBytes(const char *title, const unsigned char *bytes, size_t size);
bool hasGlobalColorTable(const header_lsd_t *header_lsd);
void printBitsOfByte(const char *title, const unsigned char *byteSrc);
unsigned sizeOfGlobalColorTable(const header_lsd_t *header_lsd);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		perror("Please give one and only one arg : srcFilename!\n");
		return -1;
	}

	FILE *gif_src = fopen(argv[1], "r");

	if (gif_src == NULL)
	{
		perror("Open");
		return -1;
	}

	header_lsd_t header_lsd;
	fread(&header_lsd, sizeof(char), sizeof(header_lsd_t), gif_src);
	bool hasGCT = hasGlobalColorTable(&header_lsd);
	unsigned sizeGCT = sizeOfGlobalColorTable(&header_lsd);
	if (hasGCT)
		fseek(gif_src, sizeGCT, SEEK_CUR);

	enum gif_section section = read_gif_section(gif_src);
	while (section != trailer)
	{
		printf("in while section with section : %d\n\n", section);
		switch (section)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			passDataSubBlocks(gif_src);
			break;
		case 4:

		default:
			perror("Section is yet unknown!\n");
			exit(1);
		}
		section = read_gif_section(gif_src);
	}
}

void printBytes(const char *title, const unsigned char *bytes, size_t size)
{
	printf("%s\n", title);
	for (int i = 0; i < size; ++i)
	{
		if (!(i % 8) && i)
			printf("\n");
		printf("0x%0x ", bytes[i]);
	}
	printf("\n\n");
}

bool hasGlobalColorTable(const header_lsd_t *header_lsd)
{
	return header_lsd->packed_field & (1 << 7);
}

void printBitsOfByte(const char *title, const unsigned char *byteSrc)
{
	unsigned char byte = *byteSrc;
	printf("%s\n", title);
	for (int i = 0; i < 8; i++)
	{
		if (!(i % 4) && i)
			printf(" ");
		printf("%d", (byte & 0x80) ? 1 : 0);
		byte <<= 1;
	}
	printf("\n\n");
}

unsigned sizeOfGlobalColorTable(const header_lsd_t *header_lsd)
{
	unsigned char color_resolution = (header_lsd->packed_field & 0x70) >> 4;
	return 3 * pow(2.0, (color_resolution + 1));
}
