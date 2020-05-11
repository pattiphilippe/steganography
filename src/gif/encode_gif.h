#include <stdio.h>

void encode(const char *src_img, const char *dest, const char *src_secret);

void writeGifWithLCT(FILE *gif_src, FILE *gif_dest, FILE *secret_src);
void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT, int *lct_id);

void hide_gif(FILE *src_img, FILE *dest, FILE *src_secret, unsigned *lct_id, int *sizeGCT);

void hideLength_gif(FILE *src_img, FILE *dest, unsigned length, int *sizeGCT);
void hideLength(FILE *src, FILE *dest, unsigned length, long *curr_pos, long *max_pos);

void hideSecret_gif(FILE *src_img, FILE *dest, FILE *src_secret, int *sizeGCT);
void hideSecret(FILE *src, FILE *dest, FILE *secret, long *curr_pos, long *max_pos);


void hideBit_gif(FILE *src_img, FILE *dest, const int secret_bit, long *curr_pos);

void copyRestOfCT(FILE *src, FILE *dest, long *curr_pos, long *max_pos);