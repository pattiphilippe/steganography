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

void writeGifWithLCT(const char *source, const char *dest,  const char *secret_src_file)
{
	FILE *gif_src = fopen(source, "r");
	if (gif_src == NULL)
	{
		perror(strcat("gif.c::writeGifWithLCT : Error opening src file ", source));
		exit(1);
	}

	FILE *gif_dest = fopen(dest, "w");
	if (gif_dest == NULL)
	{
		perror(strcat("gif.c::writeGifWithLCT : Error opening dest file ", dest));
		exit(1);
	}

	FILE *secret_src = fopen(secret_src_file, "r");
	if (secret_src == NULL)
	{
		perror(strcat("gif.c::writeGifWithLCT : Error opening secret file ", secret_src));
		exit(1);
	}

	int sizeGCT = 0;
	long posGCT = 0;
	copyHeaderLsdGct(gif_src, gif_dest, &sizeGCT, &posGCT);	

	unsigned secret_length = checkLengths(gif_src, secret_src);	

	gif_section_t section = read_gif_section(gif_src, gif_dest, true);
	while (section != trailer)
	{
		switch (section)
		{
		case 0:
		case 1:
		case 2: 
			copyDataSubBlocks(gif_src, gif_dest);
			break;
		case 3:
			hideLength(gif_src, gif_dest, secret_length);
			copyDataSubBlocks(gif_src, gif_dest);
			break;
		case 4:
			copyImageDescrBlockWithLCT(gif_src, gif_dest, secret_src, sizeGCT, posGCT);
			break;
		default:
			errno = 22;
			perror("Section is yet unknown! GIF structure is bad!\n");
			exit(1);
		}
		section = read_gif_section(gif_src, gif_dest, true);
	}

	fclose(gif_src);
	fclose(gif_dest);
	fclose(secret_src);
}

unsigned checkLengths(FILE *src_img, FILE *src_secret)
{
	unsigned secret_length = get_file_length(src_secret);
	unsigned max_lct = getMaxLCT(src_img);

	printf(" + secret file length : %d\n", secret_length);
	printf(" + max lct : %d\n", max_lct);

	/*if ((secret_length *8 ) > max_lct)  //todo : nb max clt * size gct * 8 
	{
		fprintf(stderr, "Secret too large for source image!\n");
		exit(1);
	}*/
	return secret_length;
}

void hideLength(FILE *src_img, FILE *dest, unsigned length)
{
	unsigned nb_bits = sizeof(unsigned) * 8, div = 1U << (nb_bits - 1);
	for (int i = nb_bits - 1; i >= 0; i--)
	{
		hideBit(src_img, dest, (length / div));
		length %= div;
		div >>= 1;
	}
}

void hideBit(FILE *src_img, FILE *dest, const int secret_bit)
{
	char src_img_buffer = fgetc(src_img);

	int img_bit = src_img_buffer & 1;

	if (img_bit != secret_bit)
	{
		if (secret_bit == 0)
			src_img_buffer = (src_img_buffer & ~1);
		else
			src_img_buffer = (src_img_buffer | 1);
	}
	fputc(src_img_buffer, dest);
}


void hideBit2(char *src_img_buffer, FILE *dest, const int secret_bit, const int count)
{
	int img_bit = *src_img_buffer & 1;

	if (img_bit != secret_bit)
	{
		if (secret_bit == 0)
			*src_img_buffer = (*src_img_buffer & ~1);
		else
			*src_img_buffer = (*src_img_buffer | 1);
	}
	//fwrite(src_img_buffer, count, 1, dest);
	//fputc(*src_img_buffer, dest);
}

int get_bit(char byte, int bit_nb)
{
	return (byte >> (7- bit_nb)) & 1;
}