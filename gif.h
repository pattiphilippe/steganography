#include <stdbool.h> 
#include <stdio.h>

/**
 * prototype of logical screen descriptor structure :
 */
typedef struct __attribute__((__packed__)) header_lsd_t 
{
	unsigned char header[6];
	unsigned char width[2];
	unsigned char height[2];
	unsigned char packed_field;
	unsigned char data[2];
} header_lsd_t;




enum gif_section {
	graphics,
	plain_text,
	application,
	comment,
	image_descr,
	local_color_table,
	image_data,
	trailer
};



enum gif_section read_gif_section(FILE * source);

void passSection(FILE * source, enum gif_section section);
void passDataSubBlocks(const FILE * source);
