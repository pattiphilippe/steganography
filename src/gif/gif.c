#include "gif.h"

#include "../utils/bmp.h"

bool hasColorTable(const unsigned char *packed_field)
{
	return *packed_field & (1 << 7);
}

int sizeOfColorTable(const unsigned char *packed_field)
{
	return 3 * pow(2.0, ((*packed_field & 0x07) + 1));
}

unsigned check_lengths_gif(FILE *src_img, FILE *src_secret)
{
	unsigned secret_length = get_file_length(src_secret);
	int max_secret_length = getMaxSecretLength(src_img);

	if (secret_length > max_secret_length)
	{
		fprintf(stderr, "Secret too large for source gif, use maximum %d characters! (%d chars in secret)\n", max_secret_length, secret_length);
		exit(1);
	}
	return secret_length;
}

int getMaxSecretLength(FILE *gif_src)
{
	long save_pos = ftell(gif_src);
	fseek(gif_src, 0L, SEEK_SET);

	int sizeGCT = 0, maxLCT = 0;
	passHeaderLsdGct(gif_src, &sizeGCT, NULL);

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

	if(maxLCT == 1){
		perror("Error : no space for secret, only 1 possible LCT in gif! Use a bigger gif!");
		exit(1);
	}

	return (maxLCT - 1) * (sizeGCT / 8);
}

gif_section_t read_gif_section(FILE *source, FILE *dest, bool copy)
{
	unsigned char buffer;
	bool again;
	do
	{
		again = false;
		buffer = fgetc(source);
		if (copy)
			fputc(buffer, dest);
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

		if (sizeGCT != NULL)
			*sizeGCT = size;
		if (posGCT != NULL)
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
			copyGCT(source, dest, size);
		}
		else
			fseek(source, size, SEEK_CUR);
	}
}

void copyGCT(FILE *source, FILE *dest, int sizeGCT)
{
	char buffer;
	for (int i = 0; i < sizeGCT; i++)
	{
		buffer = fgetc(source);
		fputc(buffer, dest);
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
	char buffer;
	unsigned char size = fgetc(source);
	while (size)
	{
		if (copy)
		{
			fputc(size, dest); // copy size read
			for (int i = 0; i < size; i++)
			{
				buffer = fgetc(source);
				fputc(buffer, dest);
			}
		}
		else
		{
			fseek(source, size, SEEK_CUR);
		}
		size = fgetc(source);
	}
	if (copy)
		fputc(size, dest); // copy block terminator (00)
}

void passImageDescrBlock(FILE *source)
{
	image_descr_t image_descr;
	fread(&image_descr, 1, sizeof(image_descr), source);
	if (hasColorTable(&(image_descr.packed_field)))
	{
		unsigned size_lct = sizeOfColorTable(&(image_descr.packed_field));
		fseek(source, size_lct, SEEK_CUR);
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
		x >>= 1;
		div++;
	}
	image_descr->packed_field = (image_descr->packed_field & 0xf8) | div; // set size of color table in packed field
}
