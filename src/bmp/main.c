#include <stdio.h>
#include <errno.h>

#include "bitmap.h"
#include "utils.h"

void encode(FILE *src_img, unsigned src_length, FILE *src_secret, FILE *dest);
void decode();

int main(int argc, char *argv[])
{
	printf("%s", "Start program\n\n");

	check_nb_arg(argc, argv[0]);

	int mode;
	set_mode(argv[1], argv[0], &mode);

	FILE *src_img = set_open_file_mode(argv[2], READ, _ERROR_OPEN_FILE_R);
	FILE *dest = set_open_file_mode(argv[3], WRITE, _ERROR_OPEN_FILE_W);
	FILE *src_secret = set_open_file_mode(argv[4], READ, _ERROR_OPEN_FILE_R); // TODO move into encode()

	copy_header(src_img, dest);

	if (mode)
	{
		int src_data_length = get_image_data_length(src_img);
		encode(src_img, src_data_length, src_secret, dest);
	}
	else
		decode(); //decode partie : todo

	fclose(src_img);
	fclose(dest);
	fclose(src_secret);

	printf("%s", "end program\n\n");
}

void encode(FILE *src_img, unsigned src_length, FILE *src_secret, FILE *dest)
{
	int secret_length = get_message_input_length(src_secret); // TODO si pas assez, PAS erreur avant de commencer, besoin pour gif... 

	char src_img_buffer, src_msg_buffer;
	int secret_bit, img_bit, i;
	//TODO hide message length first, int value on 4 bytes should be enough => +- 2.000.000.000
	while (!feof(src_secret) && !feof(src_img)) 
	{
		src_msg_buffer = fgetc(src_secret);
		i = 1;
		while (i <= 8 && !feof(src_img))
		{
			src_img_buffer = fgetc(src_img);

			img_bit = src_img_buffer & 1;
			secret_bit = get_wanted_bit(src_msg_buffer, i);

			if (img_bit != secret_bit)
			{
				if (secret_bit == 0)
					src_img_buffer = (src_img_buffer & ~1);
				else
					src_img_buffer = (src_img_buffer | 1);
			}
			fputc(src_img_buffer, dest);
			i++;
		}
	}
	//if eof img, stop method, no errors, closings => normal case for long text with gif
	if(feof(src_secret)){
		while(!feof(src_img)){
			src_img_buffer = fgetc(src_img);
			fputc(src_img_buffer, dest);
		}
	} 
}
