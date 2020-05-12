#include <stdio.h>

void decode_gif(const char *gif_src_file, const char *secret_dest_file);

void decode_lct(FILE *gif_src, FILE *secret_dest, int * secret_length, int lct_id);

unsigned decode_length_gif(FILE *gif_src, int size_lct);

void decode_secret_gif(FILE *gif_src, FILE *secret_dest, int *secret_size, int size_lct);