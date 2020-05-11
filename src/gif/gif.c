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
	perror("Section is yet unknown! GIF structure corrupted!\n");
	exit(1);
}

void passHeaderLsdGct(FILE *source, int *sizeGCT, long *posGCT)
{
	readHeaderLsdGct(source, NULL, false, sizeGCT, posGCT);
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

		*sizeGCT = size;
		*posGCT = ftell(source);

		if (copy)
		{
			if (dest == NULL)
			{
				errno = 22;
				perror("gif_util.c::copyHeaderLSDGCT : dest = NULL");
				exit(0);
			}
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

	if (resetCUR)
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
		case 4:
			maxLCT++;
			passImageDescrBlock(gif_src);
			break;
		default:
			passDataSubBlocks(gif_src);
		}
		section = read_gif_section(gif_src, NULL, false);
	}

	fseek(gif_src, save_pos, SEEK_SET);

	return maxLCT * sizeGCT;
}
