#include "decode_bmp.h"

#include "../utils/general.h"
#include "../utils/bmp.h"

void decode_bmp(const char *bmp_src_file, const char *bmp_dest_file)
{
    FILE *bmp_src = set_open_file_mode(bmp_src_file, READ, _ERROR_OPEN_FILE_R);
    FILE *bmp_dest = set_open_file_mode(bmp_dest_file, WRITE, _ERROR_OPEN_FILE_W);

    pass_header(bmp_src);

    unsigned length = decode_length_bmp(bmp_src);
    decode_secret_bmp(bmp_src, bmp_dest, length);

    fclose(bmp_src);
    fclose(bmp_dest);
}


void pass_header(FILE *bmp_src)
{
	unsigned offset = get_bmp_offset(bmp_src);
	fseek(bmp_src, offset, SEEK_SET);
}