/**
 * prototype of logical screen descriptor structure :
 */
typedef struct header_lsd 
{
	char header[6];
	char width[2];
	char height[2];
	char packed_field;
	char data[2];
}__attribute__((packed));

 
