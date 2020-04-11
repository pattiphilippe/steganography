#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#include "bitmap.h"
#include "utils.h"

void encode(FILE *src_img, FILE *dest, const char *src_secret_file);
unsigned checkLengths(FILE *src_img, FILE *src_secret);
void hideSecret(FILE *src_img, FILE *dest, FILE *src_secret);
void decode_secret(FILE *src_img, FILE *dest, const unsigned length);
void hideLength(FILE *src_img, FILE *dest, const unsigned length);
void hideBit(FILE *src_img, FILE *dest, const int secret_bit);
void copyRestOfImage(FILE *src_img, FILE *dest);
void decode(FILE *src_img, FILE *dest);
unsigned decode_length(FILE *src_img);
int decode_bit(FILE *src_img);

int main(int argc, char *argv[])
{
	printf("Start program\n\n");

	if (argc < 4)
		printUsage(argv[0], _ERROR_NB_ARGS_);
	printf("Using src file : %s\n\n", argv[2]);

	int mode;
	set_mode(argv[0], argv[1], &mode, argc);

	FILE *src_img = set_open_file_mode(argv[2], READ, _ERROR_OPEN_FILE_R);
	FILE *dest = set_open_file_mode(argv[3], WRITE, _ERROR_OPEN_FILE_W);

	if (mode == 0)
	{
		encode(src_img, dest, argv[4]);
	}
	else
	{
		decode(src_img, dest);
	}

	fclose(src_img);
	fclose(dest);
	printf("%s", "end program\n\n\n");
}

void decode(FILE *src_img, FILE *dest)
{
	pass_header(src_img);
	unsigned length = decode_length(src_img);
	printf("length decoded : %u\n", length);
	decode_secret(src_img, dest, length);
}

void encode(FILE *src_img, FILE *dest, const char *src_secret_file)
{
	copy_header(src_img, dest);

	FILE *src_secret = set_open_file_mode(src_secret_file, READ, _ERROR_OPEN_FILE_R);

	unsigned secret_length = checkLengths(src_img, src_secret);
	hideLength(src_img, dest, secret_length);

	hideSecret(src_img, dest, src_secret);
	fclose(src_secret);

	copyRestOfImage(src_img, dest);
}

unsigned checkLengths(FILE *src_img, FILE *src_secret)
{
	unsigned secret_length = get_file_length(src_secret);
	unsigned src_data_length = get_image_data_length(src_img);

	if (((secret_length + 4) * 8) > src_data_length)
	{
		fprintf(stderr, "Secret too large for source image!\n");
		exit(1);
	}
	return secret_length;
}

unsigned decode_length(FILE *src_img)
{
	unsigned nb_bits = sizeof(unsigned) * 8, mult = 1U << (nb_bits - 1), length = 0;
	for (int i = nb_bits - 1; i >= 0; i--)
	{
		int bit = decode_bit(src_img);
		length += bit * mult;
		mult >>= 1;
	}
	return length;
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

void decode_secret(FILE *src_img, FILE *dest, const unsigned length)
{
	char dest_buffer;
	for (unsigned i = 0; i < length; i++)
	{
		dest_buffer = 0;
		for (int j = 0; j < 8; j++)
		{
			dest_buffer <<= 1;
			int bit = decode_bit(src_img);
			if (bit == 0)
				dest_buffer = dest_buffer & ~1;
			else
				dest_buffer = dest_buffer | 1;
		}
		fputc(dest_buffer, dest);
	}
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

int decode_bit(FILE *src_img)
{
	return fgetc(src_img) & 1;
}

void hideBit(FILE *src_img, FILE *dest, const int secret_bit)
{
	char src_img_buffer = fgetc(src_img);

	int img_bit = src_img_buffer & 1;

	if (img_bit != secret_bit)
	{
		if (secret_bit == 0)
			src_img_buffer = (src_img_buffer & ~1);
		else
			src_img_buffer = (src_img_buffer | 1);
	}
	fputc(src_img_buffer, dest);
}

void copyRestOfImage(FILE *src_img, FILE *dest)
{
	char src_img_buffer = fgetc(src_img);
	while (!feof(src_img))
	{
		fputc(src_img_buffer, dest);
		src_img_buffer = fgetc(src_img);
	}
}
