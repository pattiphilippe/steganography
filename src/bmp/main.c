#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#include "bitmap.h"
#include "utils.h"

void encode(FILE *src_img, FILE *dest, const char *src_secret_file);
unsigned checkLengths(FILE *src_img, FILE *src_secret);
void hideSecret(FILE *src_img, FILE *dest, FILE *src_secret);
void hideLength(FILE *src_img, FILE *dest, const unsigned length);
void hideBit(FILE *src_img, FILE *dest, const int secret_bit);
void copyRestOfImage(FILE *src_img, FILE *dest);
void decode();

int main(int argc, char *argv[])
{
	printf("%s", "Start program\n\n");

	check_nb_arg(argc, argv[0]);

	int mode;
	set_mode(argv[1], argv[0], &mode);
	printf("argv[2] : %s\n",argv[2]);

	FILE *src_img = set_open_file_mode(argv[2], READ, _ERROR_OPEN_FILE_R);
	FILE *dest = set_open_file_mode(argv[3], WRITE, _ERROR_OPEN_FILE_W);

	copy_header(src_img, dest);

	if (mode)
	{
		encode(src_img, dest, argv[4]);
	}
	else
	{
		//TODO decode();
	}

	fclose(src_img);
	fclose(dest);
	printf("%s", "end program\n\n");
}

void encode(FILE *src_img, FILE *dest, const char *src_secret_file)
{
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

void hideLength(FILE *src_img, FILE *dest, unsigned length)
{
	unsigned nb_bits = sizeof(length) * 8, div = 1U << (nb_bits - 1);
	for (int i = nb_bits - 1; i >= 0; i--)
	{
		hideBit(src_img, dest, (length / div));
		length %= div;
		div >>= 1;
	}
}

void hideSecret(FILE *src_img, FILE *dest, FILE *src_secret)
{
	printf("ftell(src_img) : %ld\n", ftell(src_img));
	char src_msg_buffer = fgetc(src_secret);
	int secret_bit;
	while (!feof(src_secret))
	{
		for (int i = 1; i <= 8; i++) //TODO check with decode if it encodes the message correctly
		{
			secret_bit = get_bit(src_msg_buffer, i);
			hideBit(src_img, dest, secret_bit);
		}
		src_msg_buffer = fgetc(src_secret);
	}
	printf("ftell(src_img) : %ld\n", ftell(src_img));
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
