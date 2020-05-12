#include "encode_bmp.h"

#include "../utils/general.h"
#include "../utils/bmp.h"

void encode(const char *bmp_src_file, const char *bmp_dest_file, const char *secret_src_file)
{
    FILE *src_img = set_open_file_mode(bmp_src_file, READ, _ERROR_OPEN_FILE_R);
    FILE *dest = set_open_file_mode(bmp_dest_file, WRITE, _ERROR_OPEN_FILE_W);
    FILE *secret_src = set_open_file_mode(secret_src_file, READ, _ERROR_OPEN_FILE_R);

    copy_header(src_img, dest);

    unsigned secret_length = check_lengths(src_img, secret_src);
    hide_length_bmp(src_img, dest, secret_length);

    hide_secret(src_img, dest, secret_src);

    copy_rest_of_image(src_img, dest);

    fclose(secret_src);
    fclose(src_img);
    fclose(dest);
}

void hide_secret(FILE *src_img, FILE *dest, FILE *secret_src)
{
    char src_msg_buffer = fgetc(secret_src);
    int secret_bit;
    while (!feof(secret_src))
    {
        for (int i = 0; i < 8; i++)
        {
            secret_bit = get_bit(src_msg_buffer, i);
            hide_bit(src_img, dest, secret_bit);
        }
        src_msg_buffer = fgetc(secret_src);
    }
}

unsigned check_lengths(FILE *src_img, FILE *secret_src)
{
    unsigned secret_length = get_file_length(secret_src);
    unsigned max_secret_length = get_max_secret_length_bmp(src_img);

    if (secret_length > max_secret_length)
    {
		fprintf(stderr, "Secret too large for source bmp, use maximum %d characters! (%d chars in secret)\n", max_secret_length, secret_length);
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