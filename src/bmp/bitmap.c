#include "bitmap.h"
#include <stdbool.h>

int get_image_src_offset(FILE *bmp_src_file)
{
	long save_pos = ftell(bmp_src_file);
	fseek(bmp_src_file, 10, 0);
	int offset = fgetc(bmp_src_file);
	fseek(bmp_src_file, save_pos, 0);
	return offset;
}

void copy_header(FILE *bmp_src, FILE *bmp_dest)
{
	char buffer;
	for (int i = 0; i < get_image_src_offset; i++)
	{
		buffer = fgetc(bmp_src);
		fputc(buffer, bmp_dest);
	}
}

/**
 * Returns the length of the message to hide.
 */
int get_message_input_length(FILE *fp)
{
	fseek(fp, 0L, SEEK_END);
	int length = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return length;
}

int get_image_data_length(FILE *bmp_src_file)
{
	long save_pos = ftell(bmp_src_file);
	fseek(bmp_src_file, 34, 0);
	int data_length = fgetc(bmp_src_file);
	fseek(bmp_src_file, save_pos, 0);
	return data_length;
}

/**
 * for debug purpose
 */
void fromByteToBitsDisplay(char data)
{
	bool bit[8];
	printf("%c", '[');
	for (int i = 0; i < 8; i++)
	{
		bit[i] = ((data >> i) & 0x01);
		printf("%d", bit[i]);
	}
	printf("%c", ']');
}

/**
 * Returns the nth bit of the given byte.
 */
int get_bit(char given_byte, int bit_nb)
{
	return ((given_byte >> (8 - bit_nb)) & 1);
}
