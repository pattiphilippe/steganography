#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include "steg.h"


int main(int argc, char *argv[])
{
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
		decode(argv[2], argv[3]);
	}
}
