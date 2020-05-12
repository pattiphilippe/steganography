#include "decode_gif.h"

#include "gif.h"
#include "../utils/general.h"
#include "../utils/bmp.h"

void decode_gif(const char *gif_src_file, const char *secret_dest_file)
{
    FILE *gif_src = set_open_file_mode(gif_src_file, READ, _ERROR_OPEN_FILE_R);
    FILE *secret_dest = set_open_file_mode(secret_dest_file, WRITE, _ERROR_OPEN_FILE_R);

    passHeaderLsdGct(gif_src, NULL, NULL);

    int secret_length = -1, lctId = 0;
    gif_section_t section = read_gif_section(gif_src, NULL, false);
    while (secret_length != 0 && section != trailer)
    {
        switch (section)
        {
        case 4:
            decode_lct(gif_src, secret_dest, &secret_length, lctId);
            lctId++;
            break;
        default:
            passDataSubBlocks(gif_src);
        }
        if (secret_length != 0)
            section = read_gif_section(gif_src, NULL, false);
    }

    fclose(gif_src);
    fclose(secret_dest);
}

void decode_lct(FILE *gif_src, FILE *secret_dest, int *secret_length, int lct_id)
{
    image_descr_t image_descr;
    fread(&image_descr, 1, sizeof(image_descr), gif_src);
    if (!hasColorTable(&(image_descr.packed_field)))
    {
        perror("File structure Error : Secret not fully decoded, but missing a LCT! Encode the file first with enc mode!\n");
        exit(1);
    }
    int size_lct = sizeOfColorTable(&(image_descr.packed_field));
    if (lct_id == 0)
        *secret_length = decode_length_gif(gif_src, size_lct);
    else
        decode_secret_gif(gif_src, secret_dest, secret_length, size_lct);

    fseek(gif_src, 1, SEEK_CUR); // in image data, passing LZW minimum code size byte
    passDataSubBlocks(gif_src);
}

unsigned decode_length_gif(FILE *gif_src, int size_lct)
{
    long curr_pos = ftell(gif_src);
    long max_pos = ftell(gif_src) + size_lct;

    unsigned length = decode_length_bmp(gif_src);

    curr_pos = ftell(gif_src);                    // equivalent to: curr_pos += 32;
    fseek(gif_src, max_pos - curr_pos, SEEK_CUR); //pass rest of color table
    return length;
}

void decode_secret_gif(FILE *gif_src, FILE *gif_dest, int *secret_length, int size_lct)
{
    int bytes_to_decode_in_lct = *secret_length >= size_lct / 8 ? size_lct / 8 : *secret_length;

    decode_secret_bmp(gif_src, gif_dest, bytes_to_decode_in_lct);

    if (*secret_length >= (size_lct / 8))
        *secret_length -= (size_lct / 8);
    else
        *secret_length = 0;
}