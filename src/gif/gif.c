#include "gif.h"

bool hasColorTable(const unsigned char *packed_field)
{
	return *packed_field & (1 << 7);
}
unsigned sizeOfColorTable(const unsigned char *packed_field)
{
	return 3 * pow(2.0, ((*packed_field & 0x07) + 1));
}

gif_section_t read_gif_section(FILE *source, FILE *dest, bool copy)
{
	unsigned char buffer;
	bool again;
	do
	{
		again = false;
		fread(&buffer, 1, 1, source);
		if (copy)
			fwrite(&buffer, 1, 1, dest);
		switch (buffer)
		{
		case 0X2C:
			return image_descr;
		case 0X21:
			again = true;
			break;
		case 0Xf9:
			return graphics;
		case 0X01:
			return plain_text;
		case 0Xff:
			return application;
		case 0Xfe:
			return comment;
		case 0X3B:
			return trailer;
		}
	} while (again);
	errno = 22;
	perror("unknown section");
	exit(0);
}

void passHeaderLsdGct(FILE *source)
{
	readHeaderLsdGct(source, NULL, false, NULL, NULL);
}

void copyHeaderLsdGct(FILE *source, FILE *dest, int *sizeGCT, long *posGCT)
{
	readHeaderLsdGct(source, dest, true, sizeGCT, posGCT);
}

void readHeaderLsdGct(FILE *source, FILE *dest, bool copy, int *sizeGCT, long *posGCT)
{
	header_lsd_t header_lsd;
	fread(&header_lsd, 1, sizeof(header_lsd_t), source);
	bool hasGCT = hasColorTable(&(header_lsd.packed_field));
	if (hasGCT)
	{
		int size = sizeOfColorTable(&(header_lsd.packed_field));
		if (copy)
		{
			if (dest == NULL)
			{
				errno = 22;
				perror("gif_util.c::copyHeaderLSDGCT : dest = NULL");
				exit(0);
			}
			*sizeGCT = size;
			*posGCT = ftell(source);
			fwrite(&header_lsd, 1, sizeof(header_lsd), dest);
			copyGCT(source, dest, size, *posGCT, false);
		}
		fseek(source, size, SEEK_CUR);
	}
}

void copyGCT(FILE *source, FILE *dest, int sizeGCT, long posGCT, bool resetCUR)
{
	long savePos_src = ftell(source);
	long savePos_dest = ftell(dest);
	char buffer[6];

	if (resetCUR)
	{
		printf("{before : %ld, ", savePos_dest);
	}

	fseek(source, posGCT, SEEK_SET);

	for (int i = 0; i < sizeGCT; i += 6)
	{
		fread(buffer, 6, 1, source);
		fwrite(buffer, 6, 1, dest);
	}
	fseek(source, savePos_src, SEEK_SET);

	if(resetCUR) 
	{
		fseek(dest, savePos_dest, SEEK_SET);
		printf("after: %ld}\n", ftell(dest));
	}
	
}

void passDataSubBlocks(FILE *source)
{
	readDataSubBlocks(source, NULL, false);
}

void copyDataSubBlocks(FILE *source, FILE *dest)
{
	readDataSubBlocks(source, dest, true);
}

void readDataSubBlocks(FILE *source, FILE *dest, bool copy)
{
	unsigned char size;
	char buffer;
	fread(&size, 1, 1, source);
	while (size)
	{
		if (copy)
		{
			fwrite(&size, 1, 1, dest); //copy size read
			for (int i = 0; i < size; i++)
			{
				fread(&buffer, 1, 1, source);
				fwrite(&buffer, 1, 1, dest);
			}
		}
		else
		{
			fseek(source, size, SEEK_CUR);
		}
		fread(&size, 1, 1, source);
	}
	if (copy)
		fwrite(&size, 1, 1, dest); //copy block terminator (00)
}

void passImageDescrBlock(FILE *source)
{
	image_descr_t image_descr;
	fread(&image_descr, 1, sizeof(image_descr), source);
	if (hasColorTable(&(image_descr.packed_field)))
	{
		unsigned sizeLCT = sizeOfColorTable(&(image_descr.packed_field));
		fseek(source, sizeLCT, SEEK_CUR);
	}
	fseek(source, 1, SEEK_CUR); // in image data, passing LZW minimum code size byte
	passDataSubBlocks(source);
}

void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT, int *lct_id)
{
	image_descr_t image_descr;
	char buffer;
	bool hasCopyGCT;
	
	fread(&image_descr, 1, sizeof(image_descr), source);
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

	if (lctID == 1)
	{
		//cacher taille message
		unsigned secret_length = checkLengths_gif(source, secret, &sizeLCT);
		hideLength_gif(source, dest, &secret_length, &sizeLCT, hasCopyGCT);
	} 
	else 
	{
		//cacher message
		hideSecret_gif(source, dest, secret, &sizeLCT, hasCopyGCT); 
	}
	
	//réécrire image data
	fread(&buffer, 1, 1, source); // in image data, copying LZW minimum code size byte
	fwrite(&buffer, 1, 1, dest);
	copyDataSubBlocks(source, dest);
}

void setPackedFieldLikeGCT(image_descr_t *image_descr, int sizeGCT)
{
	image_descr->packed_field = image_descr->packed_field | 0x80; // has LCT set to 1
	int x = (sizeGCT / 3), div = 0;
	while (x != 2)
	{
		x /= 2;
		div++;
	}
	image_descr->packed_field = (image_descr->packed_field & 0xf8) | div; // set size of color table in packed field
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