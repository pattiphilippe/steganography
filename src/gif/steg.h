#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gif.h"

void writeGifWithLCT(FILE *gif_src, FILE *gif_dest, FILE *secret_src);
int getMaxSecretLength(FILE *source);

void hideSecret(FILE *src, FILE *dest, FILE *secret, long *curr_pos, long *max_pos);

void hideSecret_gif(FILE *src_img, FILE *dest, FILE *src_secret, int *sizeGCT);
void showSecret_gif(FILE *src_img, FILE *dest, int *sizeLCT, int *secret_size);

void copyRestOfCT(FILE *src, FILE *dest, long *curr_pos, long *max_pos);
void passRestOfCT(FILE *src, long *curr_pos, long *max_pos);

void hideBit_gif(FILE *src_img, FILE *dest, const int secret_bit, long *curr_pos);
int decodeBit_gif(FILE *src_img, long *curr_pos);

void hideLength(FILE *src, FILE *dest, unsigned *length, long *curr_pos, long *max_pos);
void showLength(FILE *src, unsigned *length, long *curr_pos, long *max_pos);

void hideLength_gif(FILE *src_img, FILE *dest, unsigned *length, int *sizeGCT);

void hide_gif(FILE *src_img, FILE *dest, FILE *src_secret, unsigned *lct_id, int *sizeGCT);
void show_gif(FILE *src, FILE *dest, unsigned *lct_id, int *sizeGCT);

unsigned checkLengths_gif(FILE *src_img, FILE *src_secret);

//test : pour éviter inclusion mutuelle ! 
void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT, int *lct_id);

void encode(const char *src_img, const char *dest, const char *src_secret);
void decode(const char *src_img, const char *dest);

