#include "encode_gif.h"

#include "gif.h"

void encode(const char *src_img_file, const char *dest_file, const char *src_secret_file)
{
    FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
    FILE *dest = set_open_file_mode(dest_file, WRITE_UP, _ERROR_OPEN_FILE_W);
    FILE *src_secret = set_open_file_mode(src_secret_file, READ, _ERROR_OPEN_FILE_R);

    //todo checkLengths once
    checkLengths_gif(src_img, src_secret);
    writeGifWithLCT(src_img, dest, src_secret);

    fclose(src_img);
    fclose(dest);
    fclose(src_secret);
}

//todo check if stop creating lct when full secret has been coded
void writeGifWithLCT(FILE *gif_src, FILE *gif_dest, FILE *secret_src)
{
    int sizeGCT = 0, lctId = 0;
    long posGCT = 0;

    copyHeaderLsdGct(gif_src, gif_dest, &sizeGCT, &posGCT);

    gif_section_t section = read_gif_section(gif_src, gif_dest, true);
    while (section != trailer)
    {
        switch (section)
        {
        case 4:
            copyImageDescrBlockWithLCT(gif_src, gif_dest, secret_src, sizeGCT, posGCT, lctId);
            lctId++;
            break;
        default:
            copyDataSubBlocks(gif_src, gif_dest);
        }
        section = read_gif_section(gif_src, gif_dest, true);
    }
}

void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT, int lct_id)
{
    image_descr_t image_descr;

    fread(&image_descr, 1, sizeof(image_descr), source);

    if (!hasColorTable(&(image_descr.packed_field)))
    {
        setPackedFieldLikeGCT(&image_descr, sizeGCT);
        fwrite(&image_descr, 1, sizeof(image_descr), dest); //copy modified image descr read

        long save_pos = ftell(source);
        fseek(source, posGCT, SEEK_SET);

        hide_gif(source, dest, secret, lct_id, sizeGCT);

        fseek(source, save_pos, SEEK_SET);
    }
    else
    {
        fwrite(&image_descr, 1, sizeof(image_descr), dest); //copy image descr read

        unsigned sizeLCT = sizeOfColorTable(&(image_descr.packed_field));
        hide_gif(source, dest, secret, lct_id, sizeLCT);
    }

    char buffer;
    fread(&buffer, 1, 1, source); // in image data, copying LZW minimum code size byte
    fwrite(&buffer, 1, 1, dest);
    copyDataSubBlocks(source, dest);
}

void hide_gif(FILE *source, FILE *dest, FILE *secret, int lct_id, int sizeLCT)
{
    if (lct_id == 0)
        hideLength_gif(source, dest, secret, sizeLCT);
    else
        hideSecret_gif(source, dest, secret, sizeLCT);
}

//todo hypothese que au moins 16 couleurs diff dans gif, pour taille min d'une LCT
void hideLength_gif(FILE *src_img, FILE *dest, FILE *src_secret, int sizeGCT)
{
    unsigned secret_length = checkLengths_gif(src_img, src_secret);
    long curr_pos = ftell(src_img);
    long max_pos = curr_pos + sizeGCT;
    hide_length(src_img, dest, secret_length);

    curr_pos = ftell(src_img);
    copyRestOfCT(src_img, dest, curr_pos, max_pos);
}

void hideSecret_gif(FILE *src_gif, FILE *dest, FILE *src_secret, int sizeLCT)
{
    long curr_pos = ftell(src_gif);
    long max_pos = curr_pos + sizeLCT;
    printf("[curr pos : %ld, size lct : %d, max pos for current LCT: %ld]\n", curr_pos, sizeLCT, max_pos);

    char src_msg_buffer;
    int secret_bit;

    for (; curr_pos < max_pos; curr_pos += BYTE)
    {
        fread(&src_msg_buffer, 1, 1, src_secret);
        if (feof(src_secret)) //secret length < sizeLCT => finished hiding secret
            break;

        for (int i = 0; i < BYTE; i++)
        {
            secret_bit = get_bit(src_msg_buffer, i);
            hide_bit(src_gif, dest, secret_bit);
        }
    }

    copyRestOfCT(src_gif, dest, curr_pos, max_pos);
}

void copyRestOfCT(FILE *src, FILE *dest, long curr_pos, long max_pos)
{
    for (char src_img_buffer; curr_pos < max_pos; curr_pos++)
    {
        fread(&src_img_buffer, 1, 1, src);
        fwrite(&src_img_buffer, 1, 1, dest);
    }
}