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

void copy_header(FILE *bmp_src, FILE *bmp_dest)
{
	char buffer;
	unsigned offset = get_image_src_offset(bmp_src);
	printf("offset : %u\n", offset);
	for (int i = 0; i < offset; i++)
	{
		buffer = fgetc(bmp_src);
		fputc(buffer, bmp_dest);
	}
	printf("ftell(src) : %ld, ftell(dest) : %ld\n", ftell(bmp_src), ftell(bmp_dest));
}

/**
 * Returns the length of the message to hide.
 */
unsigned get_file_length(FILE *file)
{
	long save_pos = ftell(file);
	printf("GET FILE LENGTH  save_pos : %ld\n", save_pos);
	fseek(file, 0L, SEEK_END);
	unsigned length = ftell(file);
	fseek(file, save_pos, SEEK_SET);
	printf("ftell after : %ld\n", ftell(file));
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
int get_bit(char given_byte, int bit_nb)
{
	return ((given_byte >> (8 - bit_nb)) & 1);
}
