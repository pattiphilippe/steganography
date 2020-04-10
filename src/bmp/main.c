#include <stdio.h>
#include <errno.h>

#include "bitmap.h"
#include "utils.h"

void encode(FILE *src_img, FILE *dest, const char *src_secret_file);
void decode();

void printBitsOfByte(const char *title, const unsigned char *byteSrc);

int main(int argc, char *argv[])
{
	printf("%s", "Start program\n\n");

	check_nb_arg(argc, argv[0]);

	int mode;
	set_mode(argv[1], argv[0], &mode);

	FILE *src_img = set_open_file_mode(argv[2], READ, _ERROR_OPEN_FILE_R);
	FILE *dest = set_open_file_mode(argv[3], WRITE, _ERROR_OPEN_FILE_W);

	copy_header(src_img, dest);

	if (mode)
	{
		encode(src_img, dest, argv[4]);
	}
	else
		//TODO decode();

		fclose(src_img);
	fclose(dest);

	printf("%s", "end program\n\n");
}

void encode(FILE *src_img, FILE *dest, const char *src_secret_file)
{
	FILE *src_secret = set_open_file_mode(src_secret_file, READ, _ERROR_OPEN_FILE_R);
	int secret_length = get_file_length(src_secret);
	printf("secret_length : %d\n", secret_length);

	int src_data_length = get_image_data_length(src_img);
	printf("src_data_length : %d\n", src_data_length);

	if ((secret_length * 8) > src_data_length)
	{
		fprintf(stderr, "Secret too large for source image!\n");
		exit(1);
	}

	char src_img_buffer, src_msg_buffer;
	int secret_bit, img_bit;
	//TODO hide message length first, int value on 4 bytes should be enough => +- 2.000.000.000
	src_msg_buffer = fgetc(src_secret);
	while (!feof(src_secret))
	{
		for (int i=1; i<=8; i++)
		{
			src_img_buffer = fgetc(src_img);
			printBitsOfByte("GOOD src_img_buffer : ", &src_img_buffer);
			// printf("\n[Good src_img_buffer : ");
			// fromByteToBitsDisplay(src_img_buffer);
			// printf("  ");

			img_bit = src_img_buffer & 1;
			secret_bit = get_bit(src_msg_buffer, i);

			if (img_bit != secret_bit)
			{
				if (secret_bit == 0)
				{
					src_img_buffer = (src_img_buffer & ~1);
					// printf("secret_bit = 0 ");
					printBitsOfByte("modified and secret_bit = 0 src_img_buffer : ", &src_img_buffer);
					// printf("src_img_buffer : ");
					// fromByteToBitsDisplay(src_img_buffer);
					// printf("]\n");
				}
				else
				{
					src_img_buffer = (src_img_buffer | 1);
					printBitsOfByte("modified and secret_bit = 1 src_img_buffer : ", &src_img_buffer);
					// printf("src_img_buffer : ");
					// fromByteToBitsDisplay(src_img_buffer);
					// printf("]\n");
				}
			}
			fputc(src_img_buffer, dest);
		}
		src_msg_buffer = fgetc(src_secret);
	}

	printf("EOF src_secret : %ld\n", ftell(src_secret));
	printf("Current src_image  : %ld\n", ftell(src_img));

	src_img_buffer = fgetc(src_img);
	while (!feof(src_img))
	{
		fputc(src_img_buffer, dest);
		src_img_buffer = fgetc(src_img);
	}

	printf("EOF src_image : %ld\n", ftell(src_img));
	fclose(src_secret);
}

void printBitsOfByte(const char *title, const unsigned char *byteSrc)
{
	unsigned char byte = *byteSrc;
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
