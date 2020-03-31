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

//TODO try typedef
enum gif_section
{
	graphics,
	plain_text,
	application,
	comment,
	image_descr,
	local_color_table,
	image_data,
	trailer
};

bool hasColorTable(const unsigned char *packed_field);
unsigned sizeOfColorTable(const unsigned char *packed_field);

enum gif_section read_gif_section(FILE *source);

void passSection(FILE *source, enum gif_section section);
void passDataSubBlocks(FILE *source);
