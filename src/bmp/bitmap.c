#include "bitmap.h"
#include <stdbool.h>
#include <stdio.h>

unsigned get_image_src_offset(FILE *bmp_src_file)
{
	long save_pos = ftell(bmp_src_file);
	fseek(bmp_src_file, 10, SEEK_SET);
	unsigned offset;
	fread(&offset, 4, 1, bmp_src_file);
	fseek(bmp_src_file, save_pos, SEEK_SET);
	return offset;
}

void copy_header(FILE *bmp_src, FILE *bmp_dest)
{
	char buffer;
	unsigned offset = get_image_src_offset(bmp_src);
	printf("offset : %u\n", offset);
	for (int i = 0; i < offset; i++)
	{
		buffer = fgetc(bmp_src);
		fputc(buffer, bmp_dest);
	}
	printf("ftell(src) : %ld, ftell(dest) : %ld\n", ftell(bmp_src), ftell(bmp_dest));
}

unsigned get_file_length(FILE *file)
{
	long save_pos = ftell(file);
	printf("GET FILE LENGTH  save_pos : %ld\n", save_pos);
	fseek(file, 0L, SEEK_END);
	unsigned length = ftell(file);
	fseek(file, save_pos, SEEK_SET);
	printf("ftell after : %ld\n", ftell(file));
	return length;
}

unsigned get_image_data_length(FILE *bmp_src_file)
{
	return get_file_length(bmp_src_file) - get_image_src_offset(bmp_src_file);
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

/**
 * Returns the nth bit of the given byte.
 */
int get_bit(char given_byte, int bit_nb)
{
	return ((given_byte >> (8 - bit_nb)) & 1);
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

void encode(const char *src_img_file, const char *dest_file, const char *src_secret_file)
{
	FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
	FILE *dest = set_open_file_mode(dest_file, WRITE, _ERROR_OPEN_FILE_W);
	FILE *src_secret = set_open_file_mode(src_secret_file, READ, _ERROR_OPEN_FILE_R);

	copy_header(src_img, dest);

	unsigned secret_length = checkLengths(src_img, src_secret);

	encodeLength(src_img, dest, secret_length);
	encodeSecret(src_img, dest, src_secret);
	copyRestOfImage(src_img, dest);

	fclose(src_secret);
	fclose(src_img);
	fclose(dest);

}

void encodeLength(FILE *src_img, FILE *dest, unsigned length)
{
	int bit_msg;

	for(int i = 1; i <= 8; i++)
	{
		bit_msg = get_bit(length, i);
		encodeBit(src_img, dest, bit_msg);
	}
}

void encodeSecret(FILE *src_img, FILE *dest, FILE *src_secret)
{
	printf("ftell(src_img) : %ld\n", ftell(src_img));
	char src_msg_buffer = fgetc(src_secret);
	int secret_bit;
	while (!feof(src_secret))
	{
		for (int i = 1; i <= 8; i++) 
		{
			secret_bit = get_bit(src_msg_buffer, i);
			encodeBit(src_img, dest, secret_bit);
		}
		src_msg_buffer = fgetc(src_secret);
	}
	printf("ftell(src_img) : %ld\n", ftell(src_img));
}

void encodeBit(FILE *src_img, FILE *dest, const int secret_bit)
{
	char src_img_buffer = fgetc(src_img);
	int img_bit = src_img_buffer & 1;

	if (img_bit != secret_bit)
	{
		if (secret_bit == 0)
			src_img_buffer = (src_img_buffer ^ 1);
		else
			src_img_buffer = (src_img_buffer | 1);
	}
	fputc(src_img_buffer, dest);
}

void copyRestOfImage(FILE *src_img, FILE *dest)
{
	printf("ftell(src_img) : %ld\n", ftell(src_img));
	char src_img_buffer = fgetc(src_img);
	while (!feof(src_img))
	{
		fputc(src_img_buffer, dest);
		src_img_buffer = fgetc(src_img);
	}
}


void decode(const char *src_secret_file, const char *output_text_file)
{
	FILE *src_secret = set_open_file_mode(src_secret_file, READ, _ERROR_OPEN_FILE_R);
	FILE *output_text = set_open_file_mode(output_text_file, WRITE, _ERROR_OPEN_FILE_W);

	int size_text;

	decodeLength(src_secret, &size_text);
	decodeSecret(size_text, src_secret, output_text);

	fclose(src_secret);
	fclose(output_text);
}

void decodeLength(FILE *src_secret, int *size_text)
{
	int file_buff = 0;
	int ch;

	int offset = get_image_src_offset(src_secret);
	fseek(src_secret, offset, SEEK_SET);

	for (int i = 0; i < 8; i++)
	{
		decodeBit(src_secret, &file_buff);
	}
	*size_text = file_buff;
	printf("================================== size text  : %d\n", *size_text);
}

void decodeBit(FILE *src_secret, int *file_buff)
{
	int src_img_buffer = fgetc(src_secret);
	int bit_msg = src_img_buffer & 1;

	if (bit_msg)
		*file_buff = (*file_buff << 1) | 1;
	else
		*file_buff = *file_buff << 1;
}

void decodeSecret(int size_text, FILE *src_secret, FILE *output_text)
{
	int file_buff=0, j = 0, k = 0;
	int bit_msg;
	char output[250] = {0};

	for (int i = 0; i < (size_text * 8); i++)
	{
		j++;
		decodeBit(src_secret, &file_buff);

		if (j == 8)
		{
			putc(file_buff, output_text);
			output[k] = file_buff;
			j=0;
			file_buff = 0;
			k++;
		}
	}
}