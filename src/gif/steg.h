#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "gif.h"

void writeGifWithLCT(const char* source, const char* dest,  const char *src_secret);
int getMaxLCT(FILE *source);
unsigned checkLengths(FILE *src_img, FILE *src_secret);
int get_bit(char byte, int i);
void hideLength(FILE *src_img, FILE *dest, unsigned length);
void hideBit(FILE *src_img, FILE *dest, const int secret_bit);
void hideBit2(char *src_img_buffer, const int secret_bit);


