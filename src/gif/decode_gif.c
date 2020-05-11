#include "decode_gif.h"
#include "gif.h"

void decode(const char *src_img_file, const char *dest_secret_file)
{
    FILE *src_img = set_open_file_mode(src_img_file, READ, _ERROR_OPEN_FILE_R);
    FILE *dest_secret = set_open_file_mode(dest_secret_file, WRITE, _ERROR_OPEN_FILE_R);

    decodeLCTs(src_img, dest_secret);

    fclose(src_img);
    fclose(dest_secret);
}

void decodeLCTs(FILE *gif_src, FILE *dest_secret)
{
    int sizeGCT = 0, lctId = 0;
    long posGCT = 0;

    passHeaderLsdGct(gif_src, &sizeGCT, &posGCT);

    gif_section_t section = read_gif_section(gif_src, NULL, false);
    while (section != trailer)
    {
        printf("read section : %d\n", section);
        switch (section)
        {
        case 4:
            decodeLCT(gif_src, dest_secret, sizeGCT, posGCT, lctId);
            lctId++;
            break;
        default:
            passDataSubBlocks(gif_src);
        }
        section = read_gif_section(gif_src, NULL, false);
    }
}

//todo write in readme : no test if lct present, hypothese that u use this decode program after encode
void decodeLCT(FILE *gif_src, FILE *dest_secret, int sizeGCT, long posGCT, int lct_id)
{
    image_descr_t image_descr;
    fread(&image_descr, 1, sizeof(image_descr), gif_src);
    //todo read sizeLct from packed field?

    show_gif(gif_src, dest_secret, lct_id, sizeGCT);

    fseek(gif_src, 1, SEEK_CUR); // in image data, passing LZW minimum code size byte
    passDataSubBlocks(gif_src);
}

void show_gif(FILE *src, FILE *dest, int lct_id, int sizeGCT)
{
    static unsigned length = -1;

    if (lct_id == 0)
        length = showLength(src, sizeGCT);
    else
    {
        if (length == -1)
        {
            fprintf(stderr, "LONGUEUR DU MESSAGE EST NULLE !!!!"); //todo always print errors the same way
            exit(1);
        }
        printf("length before : %d\n", length);
        showSecret_gif(src, dest, sizeGCT, &length);
    }
}

unsigned showLength(FILE *src_gif, int sizeGCT)
{
    long curr_pos = ftell(src_gif);
    long max_pos = ftell(src_gif) + sizeGCT;

    unsigned nb_bits = sizeof(unsigned) * 8, length = 0;
    for (int i = nb_bits - 1, bit = 0; i >= 0; i--)
    {
        bit = decode_bit(src_gif);
        length <<= 1;
        length += bit;
    }

    curr_pos = ftell(src_gif); // equivalent to: curr_pos += 32;
    fseek(src_gif, max_pos - curr_pos, SEEK_CUR); //pass rest of color table
    return length;
}

//here is the problem !
void showSecret_gif(FILE *src_img, FILE *dest, int *sizeLCT, int *secret_size)
{
    long curr_pos = ftell(src_img);
    //long max_pos = curr_pos + *sizeLCT;  //a retirer

    char dest_buffer;
    int index = *secret_size >= *sizeLCT ? *sizeLCT : *secret_size;

    for (unsigned i = 0; i < index; i++)
    {
        dest_buffer = 0;
        for (int j = 0; j < 8; j++)
        {
            dest_buffer <<= 1;
            int bit = decode_bit(src_img);
            curr_pos++;

            if (bit == 0)
                dest_buffer = dest_buffer & ~1;
            else
                dest_buffer = dest_buffer | 1;
        }
        fputc(dest_buffer, dest);
    }

    if (*secret_size >= *sizeLCT)
    {
        *secret_size -= *sizeLCT;
    }
    else
    {
        //length < sizeGCT
        *secret_size = 0;
        printf("Error in decode_gif.c::showSecret_gif : length after : %d\n", *secret_size);

        exit(1); //todo check if this error makes the program quit before end program
                 //todo always exit with 1 or -1?
    }
}