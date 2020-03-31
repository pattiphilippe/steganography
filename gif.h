#include <stdbool.h>
#include <stdio.h>
#include <math.h>

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

int getMaxLCT(const char* source);
gif_section_t read_gif_section(FILE *source);

void passHeaderLSDGCT(FILE *source);
void passDataSubBlocks(FILE *source);
void passImageDescrBlock(FILE *source);
