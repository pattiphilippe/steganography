#include "bitmap.h"
#include <stdbool.h>
#include <stdio.h>


int get_image_src_offset(FILE *bmp_src_file)
{
	/*
	long save_pos = ftell(bmp_src_file);
	fseek(bmp_src_file, 10, SEEK_SET);
	int offset = fgetc(bmp_src_file);
	fseek(bmp_src_file, save_pos, SEEK_SET);*/
	return 54;
}

void copy_header(FILE *bmp_src, FILE *bmp_dest)
{
	char buffer;
	int offset = get_image_src_offset(bmp_src);
	printf("offset : %d\n", offset);
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
int get_file_length(FILE *file)
{
	long save_pos = ftell(file);
	printf("GET FILE LENGTH  save_pos : %ld\n", save_pos);
	fseek(file, 0L, SEEK_END);
	int length = ftell(file);
	fseek(file, save_pos, SEEK_SET);
	printf("ftell after : %ld\n", ftell(file));
	return length;
}

int get_image_data_length(FILE *bmp_src_file)
{
	return get_file_length(bmp_src_file) - get_image_src_offset(bmp_src_file);
}

/**
 * for debug purpose
 */
void fromByteToBitsDisplay(char data)
{
	bool bit[8];
	printf("%c", "[");
	for (int i = 0; i < 8; i++)
	{
		bit[i] = ((data >> i) & 0x01);
		printf("%d", bit[i]);
	}
	printf("%c", "]");
}

/**
 * Returns the nth bit of the given byte.
 */
int get_bit(char given_byte, int bit_nb)
{
	return ((given_byte >> (8 - bit_nb)) & 1);
}
