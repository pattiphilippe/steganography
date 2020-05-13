#include "bmp.h"

// ENCODE FUNCTIONS

void hide_length_bmp(FILE *src_img, FILE *dest, unsigned length)
{
    unsigned nb_bits = sizeof(unsigned) * 8, div = 1U << (nb_bits - 1);
    for (int i = nb_bits - 1; i >= 0; i--)
    {
        hide_bit(src_img, dest, (length / div));
        length %= div;
        div >>= 1;
    }
}

// DECODE FUNCTIONS

unsigned decode_length_bmp(FILE *src_img)
{
    unsigned nb_bits = sizeof(unsigned) * 8, length = 0;
    for (int i = nb_bits - 1, bit = 0; i >= 0; i--)
    {
        bit = decode_bit(src_img);
        length <<= 1;
        length += bit;
    }
    return length;
}

void decode_secret_bmp(FILE *src_img, FILE *dest, const unsigned length)
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

// BIT FUNCTIONS

int get_bit(char byte, int bit_nb)
{
    return (byte >> (7 - bit_nb)) & 1;
}

#if LSB
void hide_bit(FILE *src_img, FILE *dest, const int secret_bit)
{

    char src_img_buffer = fgetc(src_img);

    int img_bit = src_img_buffer & 1; // donne val du lsb

    if (img_bit != secret_bit)
    {
        if (secret_bit == 0)
            src_img_buffer = src_img_buffer & ~1; // met le dernier bit a 0
        else
            src_img_buffer = src_img_buffer | 1; // met le dernier bit a 1
    }
    fputc(src_img_buffer, dest);
}
#else
void hide_bit(FILE *src_img, FILE *dest, const int secret_bit)
{
    char src_img_buffer = fgetc(src_img);

    int img_bit = (src_img_buffer & 0x80) >> 7; // donne val du msb

    if (img_bit != secret_bit)
    {
        if (secret_bit == 0)
            src_img_buffer = src_img_buffer & 0x7F; // met le premier bit a 0
        else
            src_img_buffer = src_img_buffer | 0x80; // met le premier bit a 1
    }
    fputc(src_img_buffer, dest);
}
#endif

#if LSB
int decode_bit(FILE *src_img)
{
    return fgetc(src_img) & 1;
}
#else
int decode_bit(FILE *src_img)
{
    return (fgetc(src_img) >> 7) & 1;
}
#endif

// FILE INFO FUNCTIONS

unsigned get_file_length(FILE *file)
{
    long save_pos = ftell(file);
    fseek(file, 0L, SEEK_END);
    unsigned length = ftell(file);
    fseek(file, save_pos, SEEK_SET);
    return length;
}

unsigned get_bmp_offset(FILE *bmp_file)
{
    long save_pos = ftell(bmp_file);
    fseek(bmp_file, 10, SEEK_SET);
    unsigned offset;
    fread(&offset, 4, 1, bmp_file);
    fseek(bmp_file, save_pos, SEEK_SET);
    return offset;
}

unsigned get_max_secret_length_bmp(FILE *bmp_file)
{
    unsigned bmp_data_length = get_file_length(bmp_file) - get_bmp_offset(bmp_file);
    return (bmp_data_length / 8) - sizeof(unsigned);
}
