#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"

#define MODE_ENC "enc"
#define MODE_DEC "dec"
#define _PROGRAMME_ "Steganograph by LSB substitution : argument number error\n"
#define _PROGRAMME_USAGE "Usage: %s [enc][dec] <source> <dest> <input text>\n enc : Write your text TO image\n dec : Read your hidden text FROM image\n"
#define _ERROR_OPEN_FILE_R "impossible to open input file %s\n"
#define _ERROR_OPEN_FILE_W "impossible to create output file %s\n"
#define READ "r"
#define WRITE "w"

/**
 *
 *  Check if the number of args is enough 
 * @abort if not the right number of args
 * @Todo reduce it to 4 and infer the name of the dest file from the src
 */
void check_nb_arg(int nbArgs, char *argv0);

/**
 * 
 * Set the program to cipher or decipher mode
 * @abort if not the arg is invalid
 */
void set_mode(char *argv1, char *argv0, int *mode);

FILE *set_open_file_mode(const char *argv, const char *mode, const char *string);
