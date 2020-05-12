#include <stdio.h>

void encode_gif(const char *gif_src_file, const char *gif_dest_file, const char *secret_src_file);

void encode_image_block(FILE *gif_src, FILE *gif_dest, FILE *secret_src, int size_gct, long pos_gct, int lct_id);

void encode_lct(FILE *gif_src, FILE *gif_dest, FILE *secret_src, int lct_id, int size_gct);

void encode_length_gif(FILE *gif_src, FILE *gif_dest, FILE* secret_src, int size_gct);

void encode_secret_gif(FILE *gif_src, FILE *gif_dest, FILE *secret_src, int size_gct);

void copy_rest_of_ct(FILE *gif_src, FILE *gif_dest, long curr_pos, long max_pos);