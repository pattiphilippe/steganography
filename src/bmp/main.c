#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#include "bitmap.h"

int main(int argc, char *argv[])
{
	printf("Start program\n\n");

	int mode;

	check_nb_arg(argc, argv[0]);
	set_mode(argv[1], argv[0], &mode);

	if (mode)
		encode(argv[2], argv[3], argv[4]);
	else
		decode(argv[2], argv[3]);

	printf("End program\n\n");
}
