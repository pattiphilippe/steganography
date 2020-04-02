#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include "util.h"

bool hasColorTable(const unsigned char *packed_field)
{
	return *packed_field & (1 << 7);
}
unsigned sizeOfColorTable(const unsigned char *packed_field)
{
	// unsigned char color_resolution = *packed_field & 0x07;
	// unsigned char color_resolution = (*packed_field & 0x70) >> 4;
	return 3 * pow(2.0, ((*packed_field & 0x07) + 1));
}

gif_section_t read_gif_section(FILE *source, FILE *dest, bool copy)
{
	unsigned char buffer;
	bool again;
	do
	{
		again = false;
		printf("in read section before reading, with copy = %d\n", copy);
		fread(&buffer, 1, 1, source);
		printf("byte of section type read : 0x%02x\n", buffer);
		if (copy)
		{
			fwrite(&buffer, 1, 1, dest);
		}
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

void passHeaderLsdGct(FILE *source)
{
	readHeaderLsdGct(source, NULL, false, NULL, NULL);
}

void copyHeaderLsdGct(FILE *source, FILE *dest, int *sizeGCT, long *posGCT)
{
	readHeaderLsdGct(source, dest, true, sizeGCT, posGCT);
}

void readHeaderLsdGct(FILE *source, FILE *dest, bool copy, int *sizeGCT, long *posGCT)
{
	header_lsd_t header_lsd;
	fread(&header_lsd, 1, sizeof(header_lsd_t), source);
	bool hasGCT = hasColorTable(&(header_lsd.packed_field));
	if (hasGCT)
	{
		int size = sizeOfColorTable(&(header_lsd.packed_field));
		if (copy)
		{
			if (dest == NULL)
			{
				errno = 22;
				perror("gif_util.c::copyHeaderLSDGCT : dest = NULL");
				exit(0);
			}
			*sizeGCT = size;
			*posGCT = ftell(source);
			fwrite(&header_lsd, 1, sizeof(header_lsd), dest);
			copyGCT(source, dest, size, *posGCT);
		}
		fseek(source, size, SEEK_CUR);
	}
}

void copyGCT(FILE *source, FILE *dest, int sizeGCT, long posGCT)
{
	long savePos = ftell(source);
	fseek(source, posGCT, SEEK_SET);
	char buffer[6];
	for (int i = 0; i < sizeGCT; i += 6)
	{
		fread(buffer, 6, 1, source);
		fwrite(buffer, 6, 1, dest);
	}
	fseek(source, savePos, SEEK_SET);
}

void passDataSubBlocks(FILE *source)
{
	readDataSubBlocks(source, NULL, false);
}

void copyDataSubBlocks(FILE *source, FILE *dest)
{
	readDataSubBlocks(source, dest, true);
}

void readDataSubBlocks(FILE *source, FILE *dest, bool copy)
{
	unsigned char size;
	char buffer;
	fread(&size, 1, 1, source);
	while (size)
	{
		if (copy)
		{
			fwrite(&size, 1, 1, dest); //copy size read
			for (int i = 0; i < size; i++)
			{
				fread(&buffer, 1, 1, source);
				fwrite(&buffer, 1, 1, dest);
			}
		}
		else
		{
			fseek(source, size, SEEK_CUR);
		}
		fread(&size, 1, 1, source);
	}
	if (copy)
		fwrite(&size, 1, 1, dest); //copy block terminator (00)
}

void passImageDescrBlock(FILE *source)
{
	image_descr_t image_descr;
	fread(&image_descr, 1, sizeof(image_descr), source);
	if (hasColorTable(&(image_descr.packed_field)))
	{
		unsigned sizeLCT = sizeOfColorTable(&(image_descr.packed_field));
		fseek(source, sizeLCT, SEEK_CUR);
	}
	fseek(source, 1, SEEK_CUR); // in image data, passing LZW minimum code size byte
	passDataSubBlocks(source);
}

void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, int sizeGCT, long posGCT)
{
	image_descr_t image_descr;
	char buffer;
	fread(&image_descr, 1, sizeof(image_descr), source);
	if (hasColorTable(&(image_descr.packed_field)))
	{
		unsigned sizeLCT = sizeOfColorTable(&(image_descr.packed_field));
		fwrite(&image_descr, 1, sizeof(image_descr), dest); //copy image descr read
		for (int i = 0; i < sizeLCT; i++)					// read and copy LCT, byte by byte
		{
			fread(&buffer, 1, 1, source);
			fwrite(&buffer, 1, 1, dest);
		}
	}
	else
	{
		printf("COPYING IMAGE SECTION - NEW LCT AS GCT\n");
		setPackedFieldLikeGCT(&image_descr, sizeGCT);
		fwrite(&image_descr, 1, sizeof(image_descr), dest); //copy modified image descr read
		copyGCT(source, dest, sizeGCT, posGCT);
		fseek(source, sizeGCT, SEEK_CUR);
	}
	fread(&buffer, 1, 1, source); // in image data, copying LZW minimum code size byte
	fwrite(&buffer, 1, 1, dest);
	copyDataSubBlocks(source, dest);
}

void setPackedFieldLikeGCT(image_descr_t *image_descr, int sizeGCT)
{
	image_descr->packed_field = image_descr->packed_field | 0x80; // has LCT set to 1
	int x = (sizeGCT / 3), div = 0;
	while (x != 2)
	{
		x /= 2;
		div++;
	}
	image_descr->packed_field = (image_descr->packed_field & 0xf8) | div; // set size of color table in packed field
}