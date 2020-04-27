#include "utils.h"

void printUsage(const char *program_name, const char *error)
{
	printf(_PROGRAM_);
	puts(error);
	printf(_PROGRAM_USAGE, program_name);
	exit(1);
}

unsigned get_file_length(FILE *file)
{
	long save_pos = ftell(file);
	fseek(file, 0L, SEEK_END);
	unsigned length = ftell(file);
	fseek(file, save_pos, SEEK_SET);
	return length;
}

int get_bit(char byte, int bit_nb)
{
	return (byte >> (7- bit_nb)) & 1;
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

FILE *set_open_file_mode(const char *argv, const char *mode, const char *error_msg)
{
	FILE *file = fopen(argv, mode);
	if (file == NULL)
	{
		fprintf(stderr, error_msg, argv);
		//perror(strcat(error_msg, argv));
		exit(1);
	}
	return file;
}

void check_nb_param(const char *program_name, const unsigned nbParam)
{
	if (nbParam < 4)
		printUsage(program_name, _ERROR_NB_ARGS_);
}

void printBitsOfByte(const char *title, const char *byteSrc)
{
	char byte = *byteSrc;
	printf("%s\n", title);
	for (int i = 0; i < 8; i++)
	{
		if (!(i % 4) && i)
			printf(" ");
		printf("%d", (byte & 0x80) ? 1 : 0);
		byte <<= 1;
	}
	printf("\n\n");
}

void showUsage(const char *name)
{
	printf("Usage : %s SOURCE DESTINATION\n\n", name);
}

void printBytesHexa(const char *title, const unsigned char *bytes, size_t size)
{
	printf("%s\n", title);
	for (int i = 0; i < size; ++i)
	{
		if (!(i % 8) && i)
			printf("\n");
		printf("0x%02x ", bytes[i]);
	}
	printf("\n\n");
}

void hideLength(FILE *src_img, FILE *dest, unsigned length)
{
	unsigned nb_bits = sizeof(unsigned) * 8, div = 1U << (nb_bits - 1);
	for (int i = nb_bits - 1; i >= 0; i--)
	{
		hideBit(src_img, dest, (length / div));
		length %= div;
		div >>= 1;
	}
}


void hideBit(FILE *src_img, FILE *dest, const int secret_bit)
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

void hideSecret(FILE *src_img, FILE *dest, FILE *src_secret)
{
	char src_msg_buffer = fgetc(src_secret);
	int secret_bit;
	while (!feof(src_secret))
	{
		for (int i = 0; i < 8; i++)
		{
			secret_bit = get_bit(src_msg_buffer, i);
			hideBit(src_img, dest, secret_bit);
		}
		src_msg_buffer = fgetc(src_secret);
	}
}


