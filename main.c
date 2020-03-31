#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "gif.h"

void showUsage(const char *name)
{
	printf("Usage : %s SOURCE DESTINATION\n\n", name);
}
void printBytesHexa(const char *title, const unsigned char *bytes, size_t size);
void printBitsOfByte(const char *title, const unsigned char *byteSrc);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		errno = 22;
		perror("Wrong usage ");
		showUsage(argv[0]);
		exit(1);
	}

	FILE *gif_src = fopen(argv[1], "r");

	if (gif_src == NULL)
	{
		perror(strcat("Open src file ", argv[1]));
		return -1;
	}
	FILE *gif_dest = fopen(argv[2], "w");

	if (gif_dest == NULL)
	{
		perror(strcat("Open dest file ", argv[2]));
		return -1;
	}

	header_lsd_t header_lsd;
	fread(&header_lsd, sizeof(char), sizeof(header_lsd_t), gif_src);
	bool hasGCT = hasColorTable(&(header_lsd.packed_field));
	if (hasGCT)
	{
		unsigned sizeGCT = sizeOfColorTable(&(header_lsd.packed_field));
		fseek(gif_src, sizeGCT, SEEK_CUR);
	}
	//TODO what if no GCT, then don't care => full of LCT, which is what we need

	enum gif_section section = read_gif_section(gif_src);
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
		{
			//has to be in block, because in c, there can't be a declaration just after a "case xxx :" => case is considered as a label
			image_descr_t image_descr;
			fread(&image_descr, sizeof(char), sizeof(image_descr), gif_src);
			// printBytesHexa("image descr : ", &image_descr, sizeof(image_descr_t));
			// printBitsOfByte("packed field of image descr : ", &(image_descr.packed_field));
			if (hasColorTable(&(image_descr.packed_field)))
			{
				unsigned sizeLCT = sizeOfColorTable(&(image_descr.packed_field));
				fseek(gif_src, sizeLCT, SEEK_CUR);
			}
			else
			{
			}
			fseek(gif_src, 1, SEEK_CUR); // in image data, passing LZW minimum code size byte
			passDataSubBlocks(gif_src);
			break;
		}
		default:
			errno = 22;
			perror("Section is yet unknown! GIF structure is bad!\n");
			exit(1);
		}
		section = read_gif_section(gif_src);
	}
}

void printBytesHexa(const char *title, const unsigned char *bytes, size_t size)
{
	printf("%s\n", title);
	for (int i = 0; i < size; ++i)
	{
		if (!(i % 8) && i)
			printf("\n");
		printf("0x%02x ", bytes[i]);
	}
	printf("\n\n");
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
