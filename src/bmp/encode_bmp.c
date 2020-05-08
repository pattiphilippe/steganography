#include "encode_bmp.h"

#include "../utils/utils.h"

void encode(const char *src_img_file, const char *dest_file, const char *src_secret_file)
{
    FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
    FILE *dest = set_open_file_mode(dest_file, WRITE, _ERROR_OPEN_FILE_W);
    FILE *src_secret = set_open_file_mode(src_secret_file, READ, _ERROR_OPEN_FILE_R);

    copy_header(src_img, dest);

    unsigned secret_length = check_lengths(src_img, src_secret);
    hide_length(src_img, dest, secret_length);

    hide_secret(src_img, dest, src_secret);

    copy_rest_of_image(src_img, dest);

    fclose(src_secret);
    fclose(src_img);
    fclose(dest);
}

void hide_length(FILE *src_img, FILE *dest, unsigned length)
{
    unsigned nb_bits = sizeof(unsigned) * BYTE, div = 1U << (nb_bits - 1);
    for (int i = nb_bits - 1; i >= 0; i--)
    {
        hideBit(src_img, dest, (length / div));
        length %= div;
        div >>= 1;
    }
}

void hide_secret(FILE *src_img, FILE *dest, FILE *src_secret)
{
    char src_msg_buffer = fgetc(src_secret);
    int secret_bit;
    while (!feof(src_secret))
    {
        for (int i = 0; i < BYTE; i++)
        {
            secret_bit = get_bit(src_msg_buffer, i);
            hideBit(src_img, dest, secret_bit);
        }
        src_msg_buffer = fgetc(src_secret);
    }
}

unsigned check_lengths(FILE *src_img, FILE *src_secret)
{
    unsigned secret_length = get_file_length(src_secret);
    unsigned src_data_length = get_bmp_data_length(src_img);

    if (((secret_length + 4) * 8) > src_data_length)
    {
        fprintf(stderr, "Secret too large for source image!\n");
        exit(1);
    }
    return secret_length;
}

void copy_header(FILE *bmp_src, FILE *bmp_dest)
{
    char buffer;
    unsigned offset = get_bmp_offset(bmp_src);
    for (int i = 0; i < offset; i++)
    {
        buffer = fgetc(bmp_src);
        fputc(buffer, bmp_dest);
    }
}

void copy_rest_of_image(FILE *src_img, FILE *dest)
{
    char src_img_buffer = fgetc(src_img);
    while (!feof(src_img))
    {
        fputc(src_img_buffer, dest);
        src_img_buffer = fgetc(src_img);
    }
}