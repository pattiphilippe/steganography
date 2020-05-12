#include <stdio.h>

#include "encode_gif.h"
#include "decode_gif.h"
#include "../utils/general.h"

int main(int argc, char *argv[])
{
	printf("[Start program]\n\n");

	check_nb_param(argv[0], argc);
	printf("Using src file : %s\n\n", argv[2]);

	int mode;
	set_mode(argv[0], argv[1], &mode, argc);

	if (mode == 0)
	{
		encode_gif(argv[2], argv[3], argv[4]);
	}
	else
	{
		decode_gif(argv[2], argv[3]);
	}
	printf("[end program]\n\n\n");
}
