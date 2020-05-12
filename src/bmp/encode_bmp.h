#include <stdio.h>

void encode(const char *bmp_src_file, const char *bmp_dest_file, const char *secret_src_file);

void hide_secret(FILE *bmp_src, FILE *bmp_dest, FILE *secret_src);

unsigned check_lengths(FILE *bmp_src, FILE *secret_src);
void copy_header(FILE *bmp_src, FILE *bmp_dest);
void copy_rest_of_image(FILE *bmp_src, FILE *bmp_dest);