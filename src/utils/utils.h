#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _PROGRAM_ "Steganography by LSB substitution\n"
#define _PROGRAM_USAGE "Usage %s : \n enc <source> <dest> <input text>\t enc : Encode text in dest with source image\n dec <source> <dest text>\t\t dec : Decode text in dest from source image\n"
#define MODE_ENC "enc"
#define MODE_DEC "dec"
#define _ERROR_NB_ARGS_ "Argument number error!\n"
#define _ERROR_UNKNOWN_MODE_ "Unknown mode!\n"
#define _ERROR_OPEN_FILE_R "Impossible to open input file %s!\n"
#define _ERROR_OPEN_FILE_W "Impossible to create output file %s!\n"
#define READ "r"
#define WRITE "w"
#define WRITE_UP "w+"
#define BYTE 8


// USAGE FUNCTIONS
void check_nb_param(const char *program_name, const unsigned nbParam);
void printUsage(const char *program_name, const char *error);

// OPEN FILE MODE FUNCTIONS
void set_mode(char *argv0, char *argv1, int *mode, int argc);
FILE *set_open_file_mode(const char *argv, const char *mode, const char *error_msg);

// BIT FUNCTIONS
int get_bit(char byte, int i);
void hide_bit(FILE *src_img, FILE *dest, const int secret_bit);
int decode_bit(FILE *src_img);

// FILE INFO FUNCTIONS
unsigned get_file_length(FILE *file);
unsigned get_bmp_offset(FILE *bmp_file);
unsigned get_bmp_data_length(FILE *bmp_file);

// PRINT FUNCTIONS
void printBytesHexa(const char *title, const unsigned char *bytes, size_t size);
void printBitsOfByte(const char *title, const char *byteSrc);


void hide_length(FILE *src_img, FILE *dest, const unsigned length);