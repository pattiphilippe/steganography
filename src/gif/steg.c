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
	FILE *gif_src = set_open_file_mode(source, READ, _ERROR_OPEN_FILE_R);
	FILE *gif_dest = set_open_file_mode(dest, WRITE, _ERROR_OPEN_FILE_W);
	FILE *secret_src = set_open_file_mode(dest, READ, _ERROR_OPEN_FILE_R);

	int sizeGCT = 0;
	long posGCT = 0;
	copyHeaderLsdGct(gif_src, gif_dest, &sizeGCT, &posGCT);	

	unsigned secret_length = checkLengths_gif(gif_src, secret_src);	

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
			//hideLength(gif_src, gif_dest, secret_length); //TODO hide Length somewhere with enough space (32 bytes)
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

unsigned checkLengths_gif(FILE *src_img, FILE *src_secret) //TODO 
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

void encode(const char *src_img, const char *dest, const char *src_secret)
{
	writeGifWithLCT(src_img, dest, src_secret);
}