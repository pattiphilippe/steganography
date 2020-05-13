#include "stdio.h"


// ENCODE FUNCTIONS
void hide_length_bmp(FILE *src_img, FILE *dest, const unsigned length);

// DECODE FUNCTIONS
unsigned decode_length_bmp(FILE *src_img);
void decode_secret_bmp(FILE *src_img, FILE *dest, const unsigned length);

// BIT FUNCTIONS
#define LSB 0// 1 : using LSB method, 0 : using MSB method
int get_bit(char byte, int i);
void hide_bit(FILE *src_img, FILE *dest, const int secret_bit);
int decode_bit(FILE *src_img);

// FILE INFO FUNCTIONS
unsigned get_file_length(FILE *file);
unsigned get_bmp_offset(FILE *bmp_file);
unsigned get_max_secret_length_bmp(FILE *bmp_file);