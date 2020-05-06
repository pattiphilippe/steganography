#include "steg.h"

int getMaxSecretLength(FILE *gif_src)
{
	long save_pos = ftell(gif_src);
	fseek(gif_src, 0L, SEEK_SET);

	int sizeGCT = 0, maxLCT = 0;
	long posGCT = 0;
	passHeaderLsdGct(gif_src, &sizeGCT, &posGCT);

	gif_section_t section = read_gif_section(gif_src, NULL, false);
	while (section != trailer)
	{
		switch (section)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			passDataSubBlocks(gif_src);
			break;
		case 4:
			maxLCT++;
			passImageDescrBlock(gif_src);
			break;
		default:
			errno = 22;
			perror("Section is yet unknown! GIF structure is bad!\n");
			exit(1);
		}
		section = read_gif_section(gif_src, NULL, false);
	}

	fseek(gif_src, save_pos, SEEK_SET);

	return maxLCT * sizeGCT;
}

//TODO marquer hypothèse au moins 2 lct, gif pas seulement une image
unsigned checkLengths_gif(FILE *src_img, FILE *src_secret) //TODO à check
{
	unsigned secret_length = get_file_length(src_secret);
	int max_secret_length = getMaxSecretLength(src_img);

	printf("secret file length : %d\n", secret_length);

	if (((secret_length + sizeof(unsigned)) * 8) > max_secret_length)
	{
		fprintf(stderr, "Secret too large for source image!\n");
		exit(1);
	}
	return secret_length;
}

void writeGifWithLCT(FILE *gif_src, FILE *gif_dest, FILE *secret_src) //TODO à nettoyer
{
	int sizeGCT = 0, lctId = 0;
	long posGCT = 0;

	bool encryption = secret_src != NULL;
	if (encryption)
		copyHeaderLsdGct(gif_src, gif_dest, &sizeGCT, &posGCT);
	else
		passHeaderLsdGct(gif_src, &sizeGCT, &posGCT);

	gif_section_t section = read_gif_section(gif_src, gif_dest, encryption);
	while (section != trailer)
	{
		switch (section)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			if (encryption)
				copyDataSubBlocks(gif_src, gif_dest);
			else
				passDataSubBlocks(gif_src);
			break;
		case 4:
			copyImageDescrBlockWithLCT(gif_src, gif_dest, secret_src, sizeGCT, posGCT, &lctId);
			lctId++;
			break;
		default:
			errno = 22;
			perror("Section is yet unknown! GIF structure is bad!\n");
			exit(1);
		}
		section = read_gif_section(gif_src, gif_dest, encryption);
	}
}

void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT, int *lct_id)
{
	image_descr_t image_descr;
	char buffer;
	bool encryption = secret != NULL;
	long save_pos;

	fread(&image_descr, 1, sizeof(image_descr), source);

	//ENCRYPTION
	if (encryption)
	{
		if (!hasColorTable(&(image_descr.packed_field)))
		{
			setPackedFieldLikeGCT(&image_descr, sizeGCT);
			fwrite(&image_descr, 1, sizeof(image_descr), dest); //copy modified image descr read
			
			save_pos = ftell(source);
			fseek(source, posGCT, SEEK_SET); 

			hide_gif(source, dest, secret, lct_id, &sizeGCT);

			fseek(source, save_pos, SEEK_SET);
		}
		else
		{
			fwrite(&image_descr, 1, sizeof(image_descr), dest); //copy image descr read

			unsigned sizeLCT = sizeOfColorTable(&(image_descr.packed_field));
			hide_gif(source, dest, secret, lct_id, &sizeLCT);
		}

		fread(&buffer, 1, 1, source); // in image data, copying LZW minimum code size byte
		fwrite(&buffer, 1, 1, dest);
		copyDataSubBlocks(source, dest);
	}
	//DECRYPTION
	else
	{
		show_gif(source, dest, lct_id, &sizeGCT);

		fread(&buffer, 1, 1, source); // in image data, copying LZW minimum code size byte
		passDataSubBlocks(source);
	}
}

void encode(const char *src_img_file, const char *dest_file, const char *src_secret_file)
{
	FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
	FILE *dest = set_open_file_mode(dest_file, WRITE_UP, _ERROR_OPEN_FILE_W);
	FILE *src_secret = set_open_file_mode(src_secret_file, READ, _ERROR_OPEN_FILE_R);

	checkLengths_gif(src_img, src_secret);
	writeGifWithLCT(src_img, dest, src_secret);

	fclose(src_img);
	fclose(dest);
	fclose(src_secret);
}

void decode(const char *src_img_file, const char *dest_secret_file)
{
	FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
	FILE *dest_secret = set_open_file_mode(dest_secret_file, WRITE, _ERROR_OPEN_FILE_R);

	writeGifWithLCT(src_img, dest_secret, NULL);

	fclose(src_img);
	fclose(dest_secret);
}

void hideBit_gif(FILE *src, FILE *dest, const int secret_bit, long *curr_pos)
{
	char src_buffer = fgetc(src);
	*curr_pos = ftell(src);

	int img_bit = src_buffer & 1; //donne val du lsb

	if (img_bit != secret_bit)
	{
		if (secret_bit == 0)
			src_buffer = (src_buffer & ~1);
		else
			src_buffer = (src_buffer | 1);
	}

	fputc(src_buffer, dest);
}

int decodeBit_gif(FILE *src_img, long *curr_pos)
{
	char byte = fgetc(src_img);
	int bit = byte & 1;

	*curr_pos = ftell(src_img);

	return bit;
}

void hideSecret_gif(FILE *src_img, FILE *dest, FILE *src_secret, int *sizeLCT)
{
	long curr_pos = ftell(src_img);
	long max_pos = curr_pos + *sizeLCT;

	printf("[curr pos : %ld, size lct : %d, max pos for current LCT: %ld]\n", curr_pos, *sizeLCT, max_pos);

	hideSecret(src_img, dest, src_secret, &curr_pos, &max_pos);
	copyRestOfCT(src_img, dest, &curr_pos, &max_pos);
}

//here is the problem !
void showSecret_gif(FILE *src_img, FILE *dest, int *sizeLCT, int *secret_size)
{
	long curr_pos = ftell(src_img);
	//long max_pos = curr_pos + *sizeLCT;  //a retirer

	char dest_buffer;
	int index = *secret_size >= *sizeLCT ? *sizeLCT : *secret_size;

	for (unsigned i = 0; i < index; i++)
	{ 
		dest_buffer = 0;
		for (int j = 0; j < 8; j++)
		{
			dest_buffer <<= 1;
			int bit = decodeBit_gif(src_img, &curr_pos); //reutiliser decodeBit 

			if (bit == 0)
				dest_buffer = dest_buffer & ~1;
			else
				dest_buffer = dest_buffer | 1;
		}
		fputc(dest_buffer, dest);
	}
	
	if (*secret_size >= *sizeLCT) 
	{
		*secret_size -= *sizeLCT;
	}
	else 
	{
		//length < sizeGCT
		*secret_size = 0;
		printf ("length after : %d\n", *secret_size);

		exit(0);
	}
	
}

//TODO hypothese que au moins 16 couleurs diff dans gif, pour taille min d'une LCT
void hideLength_gif(FILE *src_img, FILE *dest, unsigned *length, int *sizeGCT)
{
	long curr_pos = ftell(src_img);
	long max_pos = curr_pos + *sizeGCT;

	hideLength(src_img, dest, length, &curr_pos, &max_pos);
	copyRestOfCT(src_img, dest, &curr_pos, &max_pos);
}

void hideLength(FILE *src, FILE *dest, unsigned *length, long *curr_pos, long *max_pos)
{
	unsigned nb_bits = sizeof(unsigned) * BYTE, div = 1U << (nb_bits - 1);
	for (int i = nb_bits - 1; i >= 0; i--)
	{
		hideBit_gif(src, dest, (*length / div), curr_pos);
		*length %= div;
		div >>= 1;
	}
}

void showLength(FILE *src, unsigned *length, long *curr_pos, long *max_pos)
{
	unsigned nb_bits = sizeof(unsigned) * 8, mult = 1U << (nb_bits - 1);
	*length = 0;
	for (int i = nb_bits - 1; i >= 0; i--)
	{
		int bit = decodeBit_gif(src, curr_pos);
		*length += bit * mult;
		mult >>= 1;
	}

	passRestOfCT(src, curr_pos, max_pos);
}

void hideSecret(FILE *src, FILE *dest, FILE *secret, long *curr_pos, long *max_pos)
{
	char src_msg_buffer;
	int secret_bit;

	while (*curr_pos < *max_pos) //TODO try to change it to for, without using curr_pos and max_pos?
	{
		fread(&src_msg_buffer, 1, 1, secret);
		if (feof(secret)) //secret length < sizeLCT
			break;

		for (int i = 0; i < BYTE; i++)
		{
			secret_bit = get_bit(src_msg_buffer, i);
			hideBit_gif(src, dest, secret_bit, curr_pos);
		}
	}
}

void hide_gif(FILE *source, FILE *dest, FILE *secret, unsigned *lct_id, int *sizeLCT)
{
	if (*lct_id == 0)
	{
		//cacher taille message
		unsigned secret_length = checkLengths_gif(source, secret);
		hideLength_gif(source, dest, &secret_length, sizeLCT);
	}
	else
	{
		//cacher message
		hideSecret_gif(source, dest, secret, sizeLCT);
	}
}

void show_gif(FILE *src, FILE *dest, unsigned *lct_id, int *sizeGCT)
{
	static unsigned length = -1;

	if (*lct_id == 0) //si 1ère lct
	{
		long curr_pos = ftell(src);
		long max_pos = ftell(src) + *sizeGCT;

		showLength(src, &length, &curr_pos, &max_pos);
	}
	else
	{
		if (length == -1)
		{
			fprintf(stderr, "LONGUEUR DU MESSAGE EST NULLE !!!!"); //error
			exit(1);
		} 
		else
		{
			printf ("length before : %d\n", length);
			showSecret_gif(src, dest, sizeGCT, &length);
		}	
	}
}

void copyRestOfCT(FILE *src, FILE *dest, long *curr_pos, long *max_pos)
{
	char src_img_buffer;

	while (*curr_pos < *max_pos)
	{
		fread(&src_img_buffer, 1, 1, src);
		fwrite(&src_img_buffer, 1, 1, dest);
		*curr_pos = ftell(src);
	}
}

void passRestOfCT(FILE *src, long *curr_pos, long *max_pos)
{
	char buffer;
	while (*curr_pos < *max_pos) //pour arriver à la fin de la lct
	{
		fread(&buffer, 1, 1, src);
		*curr_pos = ftell(src);
	}
}
