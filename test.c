#include "test.h"
#include <stdbool.h>

/**
 * returns the position at which the offset is.
 * It starts after the 10 first bytes (11th).
 */
int get_image_src_offset(FILE* bmp_offset)
{
	int indicator = fseek(bmp_offset, 10, 0);
	int offset = fgetc(bmp_offset);
	return offset;
}

/**
 * returns the size of the message to hide
 */
int get_message_input_length(FILE* fp)
{
	int indicator = fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	return ftell(fp);
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
		/*bit[i] = data >> i;
		printf("bit at pos %d : %d",i, bit[i]);
		bit[i] = bit[i] & 0x01;
		printf("(data >> i) & 0x01) : %d", bit[i]);*/

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
	return ((given_byte>>8-wanted_bit)&1);
}
