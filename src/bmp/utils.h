#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "bitmap.h"

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


void printUsage(const char *program_name, const char *error);
void set_mode(char *argv0, char *argv1, int *mode, int argc);
FILE *set_open_file_mode(const char *argv, const char *mode, const char *string);
void check_nb_param(const unsigned nbParam);