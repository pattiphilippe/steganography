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
int get_image_src_offset(FILE* bmp_src_file);

void copy_header(FILE* bmp_src, FILE* bmp_dest);


/**
 * Return the length of the message to hide.
 * currently not saved but must be to retrieve
 * the message from the crypted image 
 * @todo save it
 * @param the given source file
 * @return the length of the message
 */
int get_message_input_length(FILE* msg_src_file);


/**
 * Returns the number of bytes of raw data in the bitmap.
 * Doesn't change the current position in the file.
 * @param the bitmap source file
 * @return the number of bytes of raw data
*/
int get_image_data_length(FILE* bmp_src_file);

/**
 * return the bit of the byte specified by wanted_bit.
 * (little endian)
 * @param the byte
 * @param the bit
 * @return the wanted bit
 */
int get_wanted_bit(char byte, int wanted_bit);

/**
 * display the bits contained in a byte as a
 * series of eight bits
 * (for debug purpose)
 */
void fromByteToBitsDisplay(char);

