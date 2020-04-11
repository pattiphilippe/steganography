#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#include "bitmap.h"

void decode(FILE *src_secret_bmp, FILE *output_txt_msg);
void decodeLength(FILE *pf1, int *size_txt);

int main(int argc, char *argv[])
{
	printf("%s", "Start program\n\n");

	check_nb_arg(argc, argv[0]);

	int mode;
	set_mode(argv[1], argv[0], &mode);

	if (mode)
	{	
		encode(argv[2], argv[3], argv[4]);
	}
	else
	{
		//decode(argv[2], argv[3]);
	}

	printf("%s", "end program\n\n");
}

void decode(FILE *src_secret_bmp, FILE *output_txt_msg)
{

}

void decodeLength(FILE *pf1, int *size_txt)
{
	int file_buff = 0, i;
	int ch, bit_msg;

	for (i = 0; i < 8; i++)
	{
		ch = fgetc(pf1);
		bit_msg = (ch & 1);
		if (bit_msg)
		{
			file_buff = (file_buff << 1) | 1;
		}
		else
		{
			file_buff = file_buff << 1;
		}
	}
	*size_txt = file_buff;
}