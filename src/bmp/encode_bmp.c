#include "encode_bmp.h"

#include "../utils/general.h"
#include "../utils/bmp.h"

void encode(const char *bmp_src_file, const char *bmp_dest_file, const char *secret_src_file)
{
    FILE *bmp_src = set_open_file_mode(bmp_src_file, READ, _ERROR_OPEN_FILE_R);
    FILE *bmp_dest = set_open_file_mode(bmp_dest_file, WRITE, _ERROR_OPEN_FILE_W);
    FILE *secret_src = set_open_file_mode(secret_src_file, READ, _ERROR_OPEN_FILE_R);

    copy_header(bmp_src, bmp_dest);

    unsigned secret_length = check_lengths(bmp_src, secret_src);
    hide_length_bmp(bmp_src, bmp_dest, secret_length);

    hide_secret(bmp_src, bmp_dest, secret_src);

    copy_rest_of_image(bmp_src, bmp_dest);

    fclose(secret_src);
    fclose(bmp_src);
    fclose(bmp_dest);
}

void hide_secret(FILE *bmp_src, FILE *bmp_dest, FILE *secret_src)
{
    char src_msg_buffer = fgetc(secret_src);
    int secret_bit;
    while (!feof(secret_src))
    {
        for (int i = 0; i < 8; i++)
        {
            secret_bit = get_bit(src_msg_buffer, i);
            hide_bit(bmp_src, bmp_dest, secret_bit);
        }
        src_msg_buffer = fgetc(secret_src);
    }
}

unsigned check_lengths(FILE *bmp_src, FILE *secret_src)
{
    unsigned secret_length = get_file_length(secret_src);
    unsigned max_secret_length = get_max_secret_length_bmp(bmp_src);

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

void copy_rest_of_image(FILE *bmp_src, FILE *bmp_dest)
{
    char buffer = fgetc(bmp_src);
    while (!feof(bmp_src))
    {
        fputc(buffer, bmp_dest);
        buffer = fgetc(bmp_src);
    }
}