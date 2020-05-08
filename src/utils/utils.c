#include "utils.h"

// USAGE FUNCTIONS

void check_nb_param(const char *program_name, const unsigned nbParam)
{
	if (nbParam < 4)
		printUsage(program_name, _ERROR_NB_ARGS_);
}

void printUsage(const char *program_name, const char *error)
{
	printf(_PROGRAM_);
	puts(error);
	printf(_PROGRAM_USAGE, program_name);
	exit(1);
}

// OPEN FILE MODE FUNCTIONS

void set_mode(char *argv0, char *argv1, int *mode, int argc)
{
	if (!strcmp(argv1, MODE_ENC))
	{
		if (argc != 5)
		{
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

FILE *set_open_file_mode(const char *argv, const char *mode, const char *error_msg)
{
	FILE *file = fopen(argv, mode);
	if (file == NULL)
	{
		fprintf(stderr, error_msg, argv);
		exit(1);
	}
	return file;
}

// BIT FUNCTIONS

int get_bit(char byte, int bit_nb)
{
	return (byte >> (7 - bit_nb)) & 1;
}

void hide_bit(FILE *src_img, FILE *dest, const int secret_bit)
{
	char src_img_buffer = fgetc(src_img);

	int img_bit = src_img_buffer & 1; //donne val du lsb

	if (img_bit != secret_bit)
	{
		if (secret_bit == 0)
			src_img_buffer = (src_img_buffer & ~1);
		else
			src_img_buffer = (src_img_buffer | 1);
	}
	fputc(src_img_buffer, dest);
}

int decode_bit(FILE *src_img)
{
	return fgetc(src_img) & 1;
}

// FILE INFO FUNCTIONS

unsigned get_file_length(FILE *file)
{
	long save_pos = ftell(file);
	fseek(file, 0L, SEEK_END);
	unsigned length = ftell(file);
	fseek(file, save_pos, SEEK_SET);
	return length;
}

unsigned get_bmp_offset(FILE *bmp_file)
{
	long save_pos = ftell(bmp_file);
	fseek(bmp_file, 10, SEEK_SET);
	unsigned offset;
	fread(&offset, 4, 1, bmp_file);
	fseek(bmp_file, save_pos, SEEK_SET);
	return offset;
}

unsigned get_bmp_data_length(FILE *bmp_file)
{
	return get_file_length(bmp_file) - get_bmp_offset(bmp_file);
}

// PRINT FUNCTIONS

void printBitsOfByte(const char *title, const char *byteSrc)
{
	char byte = *byteSrc;
	printf("%s\n", title);
	for (int i = 0; i < BYTE; i++)
	{
		if (!(i % 4) && i)
			printf(" ");
		printf("%d", (byte & 0x80) ? 1 : 0);
		byte <<= 1;
	}
	printf("\n\n");
}

void printBytesHexa(const char *title, const unsigned char *bytes, size_t size)
{
	printf("%s\n", title);
	for (int i = 0; i < size; ++i)
	{
		if (!(i % BYTE) && i)
			printf("\n");
		printf("0x%02x ", bytes[i]);
	}
	printf("\n\n");
}