#include "decode_bmp.h"

#include "../utils/utils.h"

void decode(const char *src_img_file, const char *dest_file)
{
    FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
    FILE *dest = set_open_file_mode(dest_file, WRITE, _ERROR_OPEN_FILE_W);

    pass_header(src_img);

    unsigned length = decode_length(src_img);
    printf("length decoded : %u\n", length);
    decode_secret(src_img, dest, length);

    fclose(src_img);
    fclose(dest);
}

unsigned decode_length(FILE *src_img)
{
    unsigned nb_bits = sizeof(unsigned) * 8, mult = 1U << (nb_bits - 1), length = 0;
    for (int i = nb_bits - 1; i >= 0; i--)
    {
        int bit = decode_bit(src_img);
        length += bit * mult;
        mult >>= 1;
    }
    return length;
}

void decode_secret(FILE *src_img, FILE *dest, const unsigned length)
{
    char dest_buffer;
    for (unsigned i = 0; i < length; i++)
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
    }
}

void pass_header(FILE *bmp_src)
{
	unsigned offset = get_bmp_offset(bmp_src);
	fseek(bmp_src, offset, SEEK_SET);
}