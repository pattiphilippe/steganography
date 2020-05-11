#include <stdio.h>


void decode(const char *src_img, const char *dest);

void decodeLCTs(FILE * src_img, FILE* dest_secret);

void decodeLCT(FILE *gif_src, FILE *dest_secret, int sizeGCT, long posGCT, int *lct_id);

void show_gif(FILE *src, FILE *dest, int *lct_id, int *sizeGCT);

unsigned showLength(FILE *src, long *curr_pos, long *max_pos);
void showSecret_gif(FILE *src_img, FILE *dest, int *sizeLCT, int *secret_size);

int decodeBit_gif(FILE *src_img, long *curr_pos);

void passRestOfCT(FILE *src, long *curr_pos, long *max_pos);