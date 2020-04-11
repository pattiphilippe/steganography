#include "bitmap.h"
#include <stdbool.h>
#include <stdio.h>

unsigned get_image_src_offset(FILE *bmp_src_file)
{
	long save_pos = ftell(bmp_src_file);
	fseek(bmp_src_file, 10, SEEK_SET);
	unsigned offset;
	fread(&offset, 4, 1, bmp_src_file);
	fseek(bmp_src_file, save_pos, SEEK_SET);
	return offset;
}

void pass_header(FILE *bmp_src)
{
	unsigned offset = get_image_src_offset(bmp_src);
	fseek(bmp_src, offset, SEEK_SET);
}

void copy_header(FILE *bmp_src, FILE *bmp_dest)
{
	char buffer;
	unsigned offset = get_image_src_offset(bmp_src);
	for (int i = 0; i < offset; i++)
	{
		buffer = fgetc(bmp_src);
		fputc(buffer, bmp_dest);
	}
}

unsigned get_file_length(FILE *file)
{
	long save_pos = ftell(file);
	fseek(file, 0L, SEEK_END);
	unsigned length = ftell(file);
	fseek(file, save_pos, SEEK_SET);
	return length;
}

unsigned get_image_data_length(FILE *bmp_src_file)
{
	return get_file_length(bmp_src_file) - get_image_src_offset(bmp_src_file);
}

void printBitsOfByte(const char *title, const char *byteSrc)
{
	char byte = *byteSrc;
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

/**
 * Returns the nth bit of the given byte.
 */
int get_bit(char byte, int bit_nb)
{
	return (byte >> (7- bit_nb)) & 1;
}
