#include <stdio.h>

void encode(const char *src_img_file, const char *dest_file, const char *src_secret_file);

void hide_secret(FILE *src_img, FILE *dest, FILE *src_secret);

unsigned check_lengths(FILE *src_img, FILE *src_secret);
void copy_header(FILE *bmp_src, FILE *bmp_dest);
void copy_rest_of_image(FILE *src_img, FILE *dest);