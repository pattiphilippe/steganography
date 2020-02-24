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
	bool hasGlobalColorTable;
	unsigned char * globalColorTable;
} header_lsd_t;


