#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


/**
 * Return the position at which the header ends.
 * In a bmp/dib file, it is at the 54th byte
 * There are 14 bytes containing file metadata
 * and 40 bytes about the raw pixel image datas
 * 
 * @param the given bmp source file
 * @return the end position of the header
 */
unsigned get_image_src_offset(FILE *bmp_src_file);

/**
 * Copy the header information from the
 * source file to the destination file.
 */
void copy_header(FILE *bmp_src, FILE *bmp_dest);

/**
 * Return the length of the message to hide.
 * currently not saved but must be to retrieve
 * the message from the crypted image 
 * @todo save it
 * @param the given source file
 * @return the length of the message
 */
unsigned get_file_length(FILE *msg_src_file);

/**
 * Returns the number of bytes of raw data in the bitmap.
 * Doesn't change the current position in the file.
 * @param the bitmap source file
 * @return the number of bytes of raw data
*/
unsigned get_image_data_length(FILE *bmp_src_file);

/**
 * Return the bit of the byte specified by o.
 * (little endian)
 * @param byte the given byte
 * @param i the bit index
 * @return the  bit
 */
int get_bit(char byte, int i);

/**
 * display the bits contained in a byte as a
 * series of eight bits
 * (for debug purpose)
 */
void printBitsOfByte(const char *title, const char *byteSrc);

/**
 * Returns the size of the secret message to hide.
 * if the size of the secret message is > than 
 * the size of the section containing raw pixels of 
 * the source image, it will print the error to 
 * the standard error stream
 * @param src_img the bitmap source image
 * @param src_secret the secret message
 * 
 */
unsigned checkLengths(FILE *src_img, FILE *src_secret);

/**
 * Encode the length of the message and the message to the bitmap file
 * 
 * @param src_img_file the bmp image source file
 * @param dest_file the bmp destination file
 * @param src_secret_file the message source file
 */
void encode(const char * src_img_file, const char* dest_file, const char *src_secret_file);

/**
 * Encode the message to the bitmap file
 * 
 * @param src_img the bmp image source file
 * @param dest the bmp destination file
 * @param src_secret the message source file
 * 
 */
void encodeSecret(FILE *src_img, FILE *dest, FILE *src_secret);

/**
 * Encode the length of the message to the bitmap file
 * 
 * @param src_img the bmp image source file
 * @param dest the bmp destination file
 * @param length the length of the message
 */
void encodeLength(FILE *src_img, FILE *dest, const unsigned length);

/**
 * Encode one given bit from the bitmap source file to the bitmap destination file
 * 
 * @param src_img the bmp image source file
 * @param dest the bmp destination file 
 * @param secret_bit the given bit
 * 
 */
void encodeBit(FILE *src_img, FILE *dest, const int secret_bit);

/**
 * Copy the rest of the bmp source file to the bmp destination file
 * from the 
 * 
 */
void copyRestOfImage(FILE *src_img, FILE *dest);

/**
 * 
 * 
 */
void decode(const char *src_secret_file, const char *output_text_msg);

/**
 * 
 * 
 */
void decodeLength(FILE *pf1, int *size_txt);

/**
 * 
 * 
 */
void decodeSecret(int size_text, FILE *file1, FILE *file2);

/**
 * 
 * 
 */
void decodeBit(FILE *src_secret, int *file_buff);
