#include "encode_gif.h"

#include "gif.h"
#include "../utils/general.h"
#include "../utils/bmp.h"

void encode_gif(const char *gif_src_file, const char *gif_dest_file, const char *secret_src_file)
{
    FILE *gif_src = set_open_file_mode(gif_src_file, READ, _ERROR_OPEN_FILE_R);
    FILE *gif_dest = set_open_file_mode(gif_dest_file, WRITE_UP, _ERROR_OPEN_FILE_W);
    FILE *secret_src = set_open_file_mode(secret_src_file, READ, _ERROR_OPEN_FILE_R);

    check_lengths_gif(gif_src, secret_src);
    int size_gct = 0, lct_id = 0;
    long pos_gct = 0;

    copyHeaderLsdGct(gif_src, gif_dest, &size_gct, &pos_gct);

    gif_section_t section = read_gif_section(gif_src, gif_dest, true);
    while (section != trailer)
    {
        switch (section)
        {
        case 4:
            encode_image_block(gif_src, gif_dest, secret_src, size_gct, pos_gct, lct_id);
            lct_id++;
            break;
        default:
            copyDataSubBlocks(gif_src, gif_dest);
        }
        section = read_gif_section(gif_src, gif_dest, true);
    }

    fclose(gif_src);
    fclose(gif_dest);
    fclose(secret_src);
}

void encode_image_block(FILE *gif_src, FILE *gif_dest, FILE *secret_src, int size_gct, long pos_gct, int lct_id)
{
    image_descr_t image_descr;

    fread(&image_descr, 1, sizeof(image_descr), gif_src);

    if (!hasColorTable(&(image_descr.packed_field)))
    {
        setPackedFieldLikeGCT(&image_descr, size_gct);
        fwrite(&image_descr, 1, sizeof(image_descr), gif_dest); //copy modified image descr read

        long save_pos = ftell(gif_src);
        fseek(gif_src, pos_gct, SEEK_SET);

        encode_lct(gif_src, gif_dest, secret_src, lct_id, size_gct);

        fseek(gif_src, save_pos, SEEK_SET);
    }
    else
    {
        fwrite(&image_descr, 1, sizeof(image_descr), gif_dest); //copy image descr read

        unsigned size_lct = sizeOfColorTable(&(image_descr.packed_field));
        encode_lct(gif_src, gif_dest, secret_src, lct_id, size_lct);
    }

    char buffer = fgetc(gif_src); // in image data, copying LZW minimum code size byte
    fputc(buffer, gif_dest);
    copyDataSubBlocks(gif_src, gif_dest);
}

void encode_lct(FILE *gif_src, FILE *gif_dest, FILE *secret_src, int lct_id, int size_lct)
{
    if (lct_id == 0)
        encode_length_gif(gif_src, gif_dest, secret_src, size_lct);
    else
        encode_secret_gif(gif_src, gif_dest, secret_src, size_lct);
}

void encode_length_gif(FILE *gif_src, FILE *gif_dest, FILE *secret_src, int size_lct)
{
    unsigned secret_length = check_lengths_gif(gif_src, secret_src);
    long curr_pos = ftell(gif_src);
    long max_pos = curr_pos + size_lct;
    hide_length_bmp(gif_src, gif_dest, secret_length);

    curr_pos = ftell(gif_src);
    copy_rest_of_ct(gif_src, gif_dest, curr_pos, max_pos);
}

void encode_secret_gif(FILE *gif_src, FILE *gif_dest, FILE *secret_src, int size_lct)
{
    long curr_pos = ftell(gif_src);
    long max_pos = curr_pos + size_lct;

    char src_msg_buffer;
    int secret_bit;

    for (; curr_pos < max_pos; curr_pos += 8)
    {
        src_msg_buffer = fgetc(secret_src);
        if (feof(secret_src)) //secret length < size_lct => finished hiding secret
            break;

        for (int i = 0; i < 8; i++)
        {
            secret_bit = get_bit(src_msg_buffer, i);
            hide_bit(gif_src, gif_dest, secret_bit);
        }
    }

    copy_rest_of_ct(gif_src, gif_dest, curr_pos, max_pos);
}

void copy_rest_of_ct(FILE *gif_src, FILE *gif_dest, long curr_pos, long max_pos)
{
    for (char buffer; curr_pos < max_pos; curr_pos++)
    {
        buffer = fgetc(gif_src);
        fputc(buffer, gif_dest);
    }
}