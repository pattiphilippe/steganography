#include "bitmap.h"

unsigned get_image_src_offset(FILE *bmp_src_file)
{
	long save_pos = ftell(bmp_src_file);
	fseek(bmp_src_file, 10, SEEK_SET);
	unsigned offset;
	fread(&offset, 4, 1, bmp_src_file);
	fseek(bmp_src_file, save_pos, SEEK_SET);
	return offset;
}

void pass_header(FILE *bmp_src)
{
	unsigned offset = get_image_src_offset(bmp_src);
	fseek(bmp_src, offset, SEEK_SET);
}

void copy_header(FILE *bmp_src, FILE *bmp_dest)
{
	char buffer;
	unsigned offset = get_image_src_offset(bmp_src);
	for (int i = 0; i < offset; i++)
	{
		buffer = fgetc(bmp_src);
		fputc(buffer, bmp_dest);
	}
}

unsigned get_image_data_length(FILE *bmp_src_file)
{
	return get_file_length(bmp_src_file) - get_image_src_offset(bmp_src_file);
}



void decode(const char *src_img_file, const char *dest_file)
{
	FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
	FILE *dest = set_open_file_mode(dest_file, WRITE, _ERROR_OPEN_FILE_W);

	pass_header(src_img);

	unsigned length = decode_length(src_img);
	printf("length decoded : %u\n", length);
	decode_secret(src_img, dest, length);

	fclose(src_img);
	fclose(dest);
}

void encode(const char * src_img_file, const char *dest_file, const char *src_secret_file)
{
	FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
	FILE *dest = set_open_file_mode(dest_file, WRITE, _ERROR_OPEN_FILE_W);
	FILE *src_secret = set_open_file_mode(src_secret_file, READ, _ERROR_OPEN_FILE_R);

	copy_header(src_img, dest);

	unsigned secret_length = checkLengths(src_img, src_secret);
	hideLength(src_img, dest, secret_length);

	hideSecret(src_img, dest, src_secret);

	copyRestOfImage(src_img, dest);

	fclose(src_secret);
	fclose(src_img);
	fclose(dest);	
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

int decode_bit(FILE *src_img)
{
	return fgetc(src_img) & 1;
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