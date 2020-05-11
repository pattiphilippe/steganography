#include <stdio.h>

//TODO always use same names for params
void decode(const char *src_img, const char *dest);

void decodeLCTs(FILE * src_img, FILE* dest_secret);

//TODO use less params for decodeLCT
void decodeLCT(FILE *gif_src, FILE *dest_secret, int * secret_length, int sizeGCT, long posGCT, int lct_id);

unsigned showLength(FILE *src_gif, int sizeGCT);
void showSecret_gif(FILE *src_img, FILE *dest, int sizeLCT, int *secret_size);