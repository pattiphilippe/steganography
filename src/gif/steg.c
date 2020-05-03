#include "steg.h"

int getMaxLCT(FILE *gif_src)
{
	long save_pos = ftell(gif_src);
	fseek(gif_src, 0L, SEEK_SET);

	passHeaderLsdGct(gif_src);

	int maxLCT = 0;
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

	return maxLCT;
}

unsigned checkLengths_gif(FILE *src_img, FILE *src_secret, int *sizeGCT) //TODO 
{
	unsigned secret_length = get_file_length(src_secret);
	//unsigned max_lct = getMaxLCT(src_img);

	printf(" + secret file length : %d\n", secret_length);
	//printf(" + max lct : %d\n", max_lct);

	/*if (secret_length > *sizeGCT) 
	{
		fprintf(stderr, "Secret too large for source image!\n");
		exit(1);
	}*/
	return secret_length;
}

//a nettoyer
void writeGifWithLCT(const char *src_file, const char *dest_file,  const char *secret_src_file, const char *mode)
{
	FILE *gif_src = set_open_file_mode(src_file, READ, _ERROR_OPEN_FILE_R);
	FILE *gif_dest = set_open_file_mode(dest_file, WRITE_UP, _ERROR_OPEN_FILE_W);

	FILE *secret_src = NULL;
	if (strcmp(mode, MODE_ENC) == 0 && secret_src_file != NULL) secret_src = set_open_file_mode(secret_src_file, READ, _ERROR_OPEN_FILE_R);
	
	int sizeGCT = 0;
	long posGCT = 0;
	if (strcmp(mode, MODE_ENC) == 0)
	{
		copyHeaderLsdGct(gif_src, gif_dest, &sizeGCT, &posGCT);	
	}
	else
	{
		passHeaderLsdGct_update(gif_src, &sizeGCT, &posGCT);
	}

	int lctId = 1;

	bool copy = false;
	if (strcmp(mode, MODE_ENC) == 0) copy = true;

	gif_section_t section = read_gif_section(gif_src, gif_dest, copy);
	while (section != trailer)
	{
		switch (section)
		{
		case 0:
		case 1:
		case 2: 
		case 3: 
			if (strcmp(mode, MODE_ENC) == 0)
			{
				copyDataSubBlocks(gif_src, gif_dest);
			} else 
			{
				passDataSubBlocks(gif_src);
			}
			break;
		case 4:
			if (strcmp(mode, MODE_ENC) == 0) 
			{
				copyImageDescrBlockWithLCT(gif_src, gif_dest, secret_src, sizeGCT, posGCT, &lctId, mode);
			}
			else
			{
				copyImageDescrBlockWithLCT(gif_src, gif_dest, NULL, sizeGCT, posGCT, &lctId, mode);
			}
			lctId++;
			break;
		default:
			errno = 22;
			perror("Section is yet unknown! GIF structure is bad!\n");
			exit(1);
		}
		section = read_gif_section(gif_src, gif_dest, copy);
	}

	fclose(gif_src);
	fclose(gif_dest);

	if (strcmp(mode, MODE_ENC) == 0 && secret_src != NULL) fclose(secret_src);
}

void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT, int *lct_id, const char *mode)
{
	image_descr_t image_descr;
	char buffer;
	bool hasCopyGCT;

	fread(&image_descr, 1, sizeof(image_descr), source);

	//ENCRYPTION
	if (strcmp(mode, MODE_ENC) == 0 && secret != NULL)
	{
		if (!hasColorTable(&(image_descr.packed_field))) // si pas de lct, copier GCT
		{
			hasCopyGCT = true;
			setPackedFieldLikeGCT(&image_descr, sizeGCT);
			fwrite(&image_descr, 1, sizeof(image_descr), dest); //copy modified image descr read
			copyGCT(source, dest, sizeGCT, posGCT, hasCopyGCT);
		}	
		else
		{
			hasCopyGCT = false;
			fwrite(&image_descr, 1, sizeof(image_descr), dest); //copy image descr read
		}
		
		unsigned sizeLCT = sizeOfColorTable(&(image_descr.packed_field));
		unsigned lctID = *lct_id;

		hide_gif(source, dest, secret, &lctID, &sizeLCT, hasCopyGCT);

		//réécrire image data
		fread(&buffer, 1, 1, source); // in image data, copying LZW minimum code size byte
		fwrite(&buffer, 1, 1, dest);
		copyDataSubBlocks(source, dest);
	}
	//DECRYPTION
	else if (strcmp(mode, MODE_DEC) == 0)
	{
		fseek(source, sizeof(image_descr), SEEK_SET); //je pointe sur ct

		printf("************************************** coucou");
		show_gif(source, dest, lct_id, &sizeGCT);

	}	
}


void encode(const char *src_img, const char *dest, const char *src_secret)
{
	writeGifWithLCT(src_img, dest, src_secret, MODE_ENC);
}

void decode(const char *src_img, const char *dest)
{
	writeGifWithLCT(src_img, dest, NULL, MODE_DEC);
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
	
	if (src == dest) fseek(src, *curr_pos -1, SEEK_SET);  //je recule d'un char pour maj

	fputc(src_buffer, dest);
}


void hideSecret_gif(FILE *src_img, FILE *dest, FILE *src_secret, int *sizeLCT, bool hasCopyGCT)
{
	long curr_pos = hasCopyGCT ? ftell(dest) : ftell(src_img);
	long max_pos = curr_pos + *sizeLCT;

	printf("[curr pos : %ld, size lct : %d, max pos for current LCT: %ld]\n", curr_pos, *sizeLCT, max_pos);
	
	if (hasCopyGCT)
	{
		//only work with dest file => where the copy of GCT is already written 
		hideSecret(dest, dest, src_secret, &curr_pos, &max_pos);
	}
	else
	{
		hideSecret(src_img, dest, src_secret, &curr_pos, &max_pos);
	}

	copyRestOfCT(src_img, dest, &curr_pos, &max_pos, hasCopyGCT);
}

void showSecret_gif(FILE *src_img, FILE *dest, int *sizeLCT, int *secret_size)
{
	long curr_pos = ftell(src_img);
	long max_pos = curr_pos + *sizeLCT;

	while (curr_pos < max_pos)
	{
		char dest_buffer;
		for (unsigned i = 0; i < *secret_size; i++)
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
			curr_pos = ftell(src_img);

			if (curr_pos == max_pos && curr_pos <= *secret_size) break;
		}
	}
}

void hideLength_gif(FILE *src_img, FILE *dest, unsigned *length, int *sizeGCT, bool hasCopyGCT)
{
	long curr_pos = hasCopyGCT ? ftell(dest) : ftell(src_img);
	long max_pos = curr_pos + *sizeGCT;

	if (hasCopyGCT)
	{
		hideLength(dest, dest, length, &curr_pos, &max_pos);
	}
	else
	{
		hideLength(src_img, dest, length, &curr_pos, &max_pos);
	}

	copyRestOfCT(src_img, dest, &curr_pos, &max_pos, hasCopyGCT);
}

void hideLength(FILE *src, FILE *dest, unsigned *length, long *curr_pos, long *max_pos)
{
	while (*curr_pos < *max_pos)
	{
		unsigned nb_bits = sizeof(unsigned) * BYTE, div = 1U << (nb_bits - 1);
		for (int i = nb_bits - 1; i >= 0; i--)
		{
			hideBit_gif(src, dest, (*length / div), curr_pos);
			*length %= div;
			div >>= 1;
		}

		if (*curr_pos == nb_bits) break;
	}
}

void showLength(FILE *src, FILE *dest, unsigned *length, long *curr_pos, long *max_pos)
{
	while (*curr_pos < *max_pos)
	{
		unsigned nb_bits = sizeof(unsigned) * 8, mult = 1U << (nb_bits - 1);
		for (int i = nb_bits - 1; i >= 0; i--)
		{
			int bit = decode_bit(src);
			*length += bit * mult;
			*curr_pos = ftell(src);
			mult >>= 1;
		}
		if (*curr_pos == nb_bits) break;
	}
	printf("***************************************** size message%d\n", *length);
}

void hideSecret(FILE *src, FILE *dest, FILE *secret, long *curr_pos, long *max_pos)
{
	char src_msg_buffer;
	int secret_bit;
	
	while (*curr_pos < *max_pos) 
	{
		fread(&src_msg_buffer, 1, 1, secret);
		if (!feof(secret))
		{
			for (int i = 0; i < BYTE; i++)
			{
				secret_bit = get_bit(src_msg_buffer, i);
				hideBit_gif(src, dest, secret_bit, curr_pos);
			}
				fread(&src_msg_buffer, 1, 1, secret);
		} 
		else 
		{
			//secret length < sizeLCT
			break;
		}
	}	
}

void hide_gif(FILE *source, FILE *dest, FILE *secret,  unsigned *lct_id, int *sizeLCT, bool hasCopyGCT)
{
	if (*lct_id == 1)
	{
		//cacher taille message
		unsigned secret_length = checkLengths_gif(source, secret, sizeLCT);
		hideLength_gif(source, dest, &secret_length, sizeLCT, hasCopyGCT);
	} 
	else 
	{
		//cacher message
		hideSecret_gif(source, dest, secret, sizeLCT, hasCopyGCT); 
	}
}
void show_gif(FILE *src, FILE *dest, unsigned *lct_id, int *sizeGCT)
{
	unsigned length = 0;
	if (*lct_id == 1)
	{
		printf("****************************  1rst lct");
		showLength(src, dest, &length, ftell(src), ftell(src) + *sizeGCT);
	} else
	{
		if (length == 0) fprintf(stderr, "================+> LONGUEUR DU MESSAGE EST NULLE !!!!");//error 
		showSecret_gif(src, dest, sizeGCT, &length);
	}
	
}

void copyRestOfCT(FILE *src, FILE *dest, long *curr_pos, long *max_pos, bool hasCopyGCT)
{
	char src_img_buffer;

	while (*curr_pos < *max_pos)
	{
		if (hasCopyGCT) 
		{
			//GCT already written in dest => pass it
			fseek(dest, 1, SEEK_CUR);
			*curr_pos = ftell(dest);
		}
		else
		{
			//copy rest of LCT from src to dest
			fread(&src_img_buffer, 1, 1, src);
			fwrite(&src_img_buffer, 1, 1, dest);
			*curr_pos = ftell(src);
		}	
	}
}

