#include <stdio.h>

void decode(const char *src_img_file, const char *dest_file);

unsigned decode_length(FILE *src_img);
void decode_secret(FILE *src_img, FILE *dest, const unsigned length);

void pass_header(FILE *bmp_src);