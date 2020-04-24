#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/utils.h"
/**
 * Logical screen descriptor structure
 */
typedef struct __attribute__((__packed__)) header_lsd_t
{
	unsigned char header[6];
	unsigned char width[2];
	unsigned char height[2];
	unsigned char packed_field;
	unsigned char data[2];
} header_lsd_t;

/**
 * Image descriptor structure
 */
typedef struct __attribute__((__packed__)) image_descr_t
{
	unsigned char left_pos[2];
	unsigned char top_pos[2];
	unsigned char width[2];
	unsigned char height[2];
	unsigned char packed_field;
} image_descr_t;

typedef enum gif_section_t
{
	graphics,
	plain_text,
	application,
	comment,
	image_descr,
	trailer
} gif_section_t;

bool hasColorTable(const unsigned char *packed_field);
unsigned sizeOfColorTable(const unsigned char *packed_field);
gif_section_t read_gif_section(FILE *source, FILE *dest, bool copy);


void passHeaderLsdGct(FILE *source);
void copyHeaderLsdGct(FILE *source, FILE *dest, int *sizeGCT, long *posGCT);
void readHeaderLsdGct(FILE *source, FILE *dest, bool copy, int *sizeGCT, long *posGCT);

void copyGCT(FILE *source, FILE *dest, int sizeGCT, long posGCT);

void copyGCTEnc(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT);

void passDataSubBlocks(FILE *source);
void copyDataSubBlocks(FILE *source, FILE *dest);
void readDataSubBlocks(FILE *source, FILE *dest, bool copy);

void passImageDescrBlock(FILE *source);
void copyImageDescrBlockWithLCT(FILE *source, FILE *dest, FILE *secret, int sizeGCT, long posGCT);

void setPackedFieldLikeGCT(image_descr_t *image_descr, int sizeGCT);
