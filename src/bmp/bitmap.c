#include "bitmap.h"
#include <stdbool.h>

int get_image_src_offset(FILE* bmp_src_file)
{
	fseek(bmp_src_file, 10, 0);
	return getc(bmp_src_file);
}

/**
 * returns the size of the message to hide
 */
int get_message_input_length(FILE* fp)
{
	fseek(fp, 0L, SEEK_END);
	int res = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return res;
}

/**
 * for debug purpose
 */
void fromByteToBitsDisplay(char data)
{
	bool bit[8];
	printf("%c",'[');
	for (int i = 0; i < 8; i++)
	{
		bit[i] = ((data >> i) & 0x01);
		printf("%d", bit[i]);
	}
	printf("%c", ']');
}

/**
 * returns the wanted bit of the given byte
 */
int get_wanted_bit(char given_byte, int wanted_bit)
{
	return ((given_byte>>(8-wanted_bit))&1);
}
