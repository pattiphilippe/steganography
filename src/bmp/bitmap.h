#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Return the position at which the header ends.
 * In a bmp/dib file, it is at the 54th byte
 * There are 14 bytes containing file metadata
 * and 40 bytes about the raw pixel image datas
 * 
 * @param the given bmp source file
 * @return the end position of the header
 */
unsigned get_image_src_offset(FILE* bmp_src_file);

void copy_header(FILE* bmp_src, FILE* bmp_dest);


/**
 * Return the length of the message to hide.
 * currently not saved but must be to retrieve
 * the message from the crypted image 
 * @todo save it
 * @param the given source file
 * @return the length of the message
 */
unsigned get_file_length(FILE* msg_src_file);


/**
 * Returns the number of bytes of raw data in the bitmap.
 * Doesn't change the current position in the file.
 * @param the bitmap source file
 * @return the number of bytes of raw data
*/
unsigned get_image_data_length(FILE* bmp_src_file);

/**
 * Return the bit of the byte specified by o.
 * (little endian)
 * @param the byte
 * @param the bit index
 * @return the  bit
 */
int get_bit(char byte, int i);

/**
 * display the bits contained in a byte as a
 * series of eight bits
 * (for debug purpose)
 */
void printBitsOfByte(const char *title, const char *byteSrc);

