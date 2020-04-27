#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include "steg.h"

void encode(const char *src_img, const char *dest, const char *src_secret);

int main(int argc, char *argv[])
{
	/*if (argc != 3)
	{
		errno = 22;
		perror("Wrong usage ");
		showUsage(argv[0]);
		exit(1);
	}*/

	check_nb_param(argv[0], argc);
	printf("Using src file : %s\n\n", argv[2]);

	int mode;
	set_mode(argv[0], argv[1], &mode, argc);

	if (mode == 0)
	{
		encode(argv[2], argv[3], argv[4]);
	}
	else
	{
	}

	/*printf("Max Local Color Tables in source file : %d\n\n\n", getMaxLCT(argv[1]));
	writeGifWithLCT(argv[1], argv[2]);
	printf("\nMax Local Color Tables in dest file : %d\n\n\n", getMaxLCT(argv[2]));*/

}

void encode(const char *src_img, const char *dest, const char *src_secret)
{
	writeGifWithLCT(src_img, dest, src_secret);
}
