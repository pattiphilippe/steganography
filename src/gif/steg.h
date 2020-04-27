#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "gif.h"

void writeGifWithLCT(const char* source, const char* dest,  const char *src_secret);
int getMaxLCT(FILE *source);
unsigned checkLengths_gif(FILE *src_img, FILE *src_secret);

void encode(const char *src_img, const char *dest, const char *src_secret);


