#include <stdio.h>
#include <errno.h>

#include "bitmap.h"
#include "utils.h"

int main(int argc, char* argv[])
{
	printf("%s", "start program\n\n");
  
	FILE *src_img_handle = NULL;
	FILE *src_msg_handle = NULL;
	FILE *dest_hidden_img_src = NULL;

	int mode;

	check_nb_arg(argc, argv[0]);
	set_mode(argv[1], argv[0], &mode);

	src_img_handle = set_open_file_mode(src_img_handle, argv[2], READ, _ERROR_OPEN_FILE_R);
	src_msg_handle = set_open_file_mode(src_msg_handle, argv[4], READ, _ERROR_OPEN_FILE_R);
	dest_hidden_img_src = set_open_file_mode(dest_hidden_img_src, argv[3], WRITE, _ERROR_OPEN_FILE_W);

	int cursor_dest = 0;
	char tmp_c;

	int offset = get_image_src_offset(src_img_handle);

	rewind(src_img_handle);
	for (int i=0; i < offset; i++)
	{  
		tmp_c = fgetc(src_img_handle);
		fputc(tmp_c, dest_hidden_img_src);

		cursor_dest++;
	}

	char src_img_buffer;
	char src_msg_buffer;

	if (mode)
	{
		int hidden_message_length = get_message_input_length(src_msg_handle);

		while ( !feof(src_img_handle)) 
		{
			int msg_bit;  	//from msg src
			int lsb_byte; 	//from bmp src

			if (!feof(src_msg_handle)) 
			{		
				src_msg_buffer = fgetc(src_msg_handle);

				//pour chaque byte de l'image d'origine
				for (int i=1; i<=8; i++) 
				{  
					src_img_buffer = fgetc(src_img_handle);
					cursor_dest++;
					
					lsb_byte = src_img_buffer & 1;
					msg_bit = get_wanted_bit(src_msg_buffer,i);
				
					if(lsb_byte == msg_bit)
					{
						fputc(src_img_buffer, dest_hidden_img_src);
					}
					else 
					{
						if(lsb_byte == 0) 
							src_img_buffer = (src_img_buffer | 1);
						else
							src_img_buffer = (src_img_buffer & ~1);
							fputc(src_img_buffer,dest_hidden_img_src);
					}
				}
			}
			else
			{
				tmp_c = fgetc(src_img_handle);
				fputc(tmp_c, dest_hidden_img_src);

				cursor_dest++;
			}
		}
		set_close_file_mode(src_msg_handle);	
	}
	else 
	{
		 //decode partie : todo 
	}
	printf("%s", "end program\n\n");

	set_close_file_mode(src_img_handle);
	set_close_file_mode(dest_hidden_img_src);
}
