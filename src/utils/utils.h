#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MODE_ENC "enc"
#define MODE_DEC "dec"
#define _PROGRAM_ "Steganograph by LSB substitution\n"
#define _PROGRAM_USAGE "Usage: %s [enc][dec] <source> <dest> <input text>\n enc : Write your text TO image\n dec : Read your hidden text FROM image\n"
#define _ERROR_NB_ARGS_ "Argument number error!\n"
#define _ERROR_UNKNOWN_MODE_ "Unknown mode!\n"
#define _ERROR_OPEN_FILE_R "Impossible to open input file %s!\n"
#define _ERROR_OPEN_FILE_W "Impossible to create output file %s!\n"
#define READ "r"
#define WRITE "w"
#define BYTE 8


void printUsage(const char *program_name, const char *error);
void showUsage(const char *name);
void printBytesHexa(const char *title, const unsigned char *bytes, size_t size);
void printBitsOfByte(const char *title, const char *byteSrc);
void set_mode(char *argv0, char *argv1, int *mode, int argc);
FILE *set_open_file_mode(const char *argv, const char *mode, const char *error_msg);
void check_nb_param(const char *program_name, const unsigned nbParam);

unsigned get_file_length(FILE *msg_src_file);

int get_bit(char byte, int i);
void hideLength(FILE *src_img, FILE *dest, const unsigned length);
void hideBit(FILE *src_img, FILE *dest, const int secret_bit);
void hideSecret(FILE *src_img, FILE *dest, FILE *src_secret);

//test
void hideSecret2(FILE *src_img, FILE *dest, FILE *src_secret, int *sizeGCT);
void hideBit2(FILE *src_img, FILE *dest, const int secret_bit, long *curr_pos);