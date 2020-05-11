#include <stdio.h>

void encode(const char *src_img, const char *dest, const char *src_secret);

void writeGifWithLCT(FILE *gif_src, FILE *gif_dest, FILE *secret_src);
void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT, int lct_id);

void hide_gif(FILE *src_img, FILE *dest, FILE *src_secret, int lct_id, int sizeGCT);

void hideLength_gif(FILE *src_img, FILE *dest, FILE* src_secret, int sizeGCT);

void hideSecret_gif(FILE *src_img, FILE *dest, FILE *src_secret, int sizeGCT);

void copyRestOfCT(FILE *src, FILE *dest, long curr_pos, long max_pos);