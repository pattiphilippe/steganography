#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/utils.h"

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

void pass_header(FILE *bmp_src);
void copy_header(FILE *bmp_src, FILE *bmp_dest);

/**
 * Returns the number of bytes of raw data in the bitmap.
 * Doesn't change the current position in the file.
 * @param the bitmap source file
 * @return the number of bytes of raw data
*/
unsigned get_image_data_length(FILE *bmp_src_file);

void encode(const char *src_img_file, const char *dest_file, const char *src_secret_file);
unsigned checkLengths(FILE *src_img, FILE *src_secret);
void hideSecret(FILE *src_img, FILE *dest, FILE *src_secret);
void decode_secret(FILE *src_img, FILE *dest, const unsigned length);
void copyRestOfImage(FILE *src_img, FILE *dest);
void decode(const char *src_img_file, const char *dest_file);
void hideLength(FILE *src_img, FILE *dest, const unsigned length);
unsigned decode_length(FILE *src_img);
int decode_bit(FILE *src_img);
