#include <stdio.h>
#include <errno.h>

#include "test.h"

int main(int argc, char* argv[])
{
  printf("%s", "start program");
  
  FILE *file_handle = NULL;
  FILE *message_handle = NULL;
  FILE *hidden_message_handle = NULL;

  // first step : ouverture des fichiers et check des arguments
  if (argc != 5) {
	  printf("Stegano by LSB substitution \nUsage: %s [enc][dec] <source> <dest> <input text>\n enc : Write your text TO image\n dec : Read your hidden text FROM image",argv[0]);
  	exit(1);
  }
  	
  int mode;
  if (!strcmp(argv[1],"enc"))
	  mode = 1;
  else if (!strcmp(argv[1],"dec"))
	  mode = 0;
  else {
  	printf("Stegano by LSB substitution \nUsage: %s <mode> <source> <dest> <input file>\n Mode : enc = encrypt\n dec = decrypt \n",argv[0]);
	exit(1);
  }

  file_handle = fopen(argv[2], "r");
  if (file_handle == NULL) {
  	fprintf(stderr, "impossible to open input file %s\n",argv[2]);
	exit(1);
  }

  hidden_message_handle = fopen(argv[3], "w");
  if (hidden_message_handle == NULL) {
  	fprintf(stderr, "impossible to create output file %s\n",argv[3]);
	exit(1);
  }


  // second step : copier dans le fichier dest le header jusque offset
  int c=0; //repésente la position du curseur dans le fichier de dest

  char tmp_header_cpy;
  int offset = get_image_src_offset(file_handle);

  rewind(file_handle);


  for (int i=0; i < offset; i++)
  {
	  tmp_header_cpy = fgetc(file_handle);
	  fputc(tmp_header_cpy, hidden_message_handle);
	  c++;
  }


  // thirth step : on accède au message 
  char file_buffer;
  char message_buffer;

  if (mode)
  {
	message_handle=fopen(argv[4],"r");
	if (message_handle== NULL) {
		fprintf(stderr, "Can't open text input file %s\n",argv[4]);
		exit(1);
	}
	int hidden_message_length=get_message_input_length(message_handle);


	/* 
	After offset has been read and the file header has been written as is for the virgin image - 
	the length of the hidden message is written as the first byte. This length is then used while decrypting the text from the image. 
	*/
	fputc(hidden_message_length,hidden_message_handle);  //dans dest on a : header + longueur message caché
	c++;
	do {
		int bit_of_message;
		if(!feof(message_handle)) {		
			message_buffer=fgetc(message_handle);
			for(int i=1;i<=8;i++) {  //pour chaque byte de l'image d'origine
				file_buffer=fgetc(file_handle);
				c++;
				
				int file_byte_lsb = file_buffer & 1; // AND avec 1 pour avoir la valeur du lsb. ça donne 0 si LSB vaut 0 OU 1 si LSB vaut 1
				bit_of_message=get_wanted_bit(message_buffer,i);
			
				if(file_byte_lsb==bit_of_message) {
					fputc(file_buffer,hidden_message_handle);
				}
				else {
					if(file_byte_lsb==0)
						file_buffer = (file_buffer | 1);
					else
						file_buffer = (file_buffer & ~1);
						//  logic to flip the LSB bit of file_buffer and put it into a file with putc()
						fputc(file_buffer,hidden_message_handle);
					
				}
			}
		}
		else {
			tmp_header_cpy=fgetc(file_handle);
			fputc(tmp_header_cpy,hidden_message_handle);
			c++;
		}
	} while(!feof(file_handle));	
	fclose(message_handle);	
	}
	else {
		message_handle=fopen(argv[4],"w");
		if (message_handle== NULL) {
			fprintf(stderr, "Can't open text input file %s\n",argv[4]);
			exit(1);
		}
	
		/* Grab LSB of all bytes for length specified at fgetc */
		int message_length=fgetc(file_handle);
		for(int i=0;i<message_length;i++) {
			char temp_ch='\0';
			for( int j=0;j<8;j++) {
				temp_ch=temp_ch<<1;
				file_buffer=fgetc(file_handle);
				int file_byte_lsb = file_buffer & 1; 
				temp_ch|=file_byte_lsb;
			}
			fputc(temp_ch,message_handle);
		}
		fclose(message_handle);	
	}

	/* Clean up before exit */
	fclose(file_handle);
	fclose(hidden_message_handle);
}
