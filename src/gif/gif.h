#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>

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
int sizeOfColorTable(const unsigned char *packed_field);
gif_section_t read_gif_section(FILE *source, FILE *dest, bool copy);

unsigned check_lengths_gif(FILE *src_img, FILE *src_secret);
int getMaxSecretLength(FILE *source);

void passHeaderLsdGct(FILE *source, int *sizeGCT, long *posGCT);
void copyHeaderLsdGct(FILE *source, FILE *dest, int *sizeGCT, long *posGCT);
void readHeaderLsdGct(FILE *source, FILE *dest, bool copy, int *sizeGCT, long *posGCT);

void copyGCT(FILE *source, FILE *dest, int sizeGCT);

void passDataSubBlocks(FILE *source);
void copyDataSubBlocks(FILE *source, FILE *dest);
void readDataSubBlocks(FILE *source, FILE *dest, bool copy);

void passImageDescrBlock(FILE *source);
void setPackedFieldLikeGCT(image_descr_t *image_descr, int sizeGCT);