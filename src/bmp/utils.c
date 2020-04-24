#include "utils.h"

void printUsage(const char *program_name, const char *error)
{
	printf(_PROGRAM_);
	puts(error);
	printf(_PROGRAM_USAGE, program_name);
	exit(1);
}

void set_mode(char *argv0, char *argv1, int *mode, int argc)
{
	if (!strcmp(argv1, MODE_ENC))
	{
		if (argc != 5)
		{
			printf("COUCOU\n");
			printUsage(argv0, _ERROR_NB_ARGS_);
		}
		*mode = 0;
	}
	else if (!strcmp(argv1, MODE_DEC))
	{
		if (argc != 4)
		{
			printUsage(argv0, _ERROR_NB_ARGS_);
		}
		*mode = 1;
	}
	else
	{
		printUsage(argv0, _ERROR_UNKNOWN_MODE_);
	}
}

FILE *set_open_file_mode(const char *argv, const char *mode, const char *string)
{
	FILE *file = fopen(argv, mode);
	if (file == NULL)
	{
		fprintf(stderr, string, argv);
		exit(1);
	}
	return file;
}

void check_nb_param(const unsigned nbParam)
{
	if (nbParam < 4)
		printUsage(nbParam, _ERROR_NB_ARGS_);
}