#include "utils.h"


void check_nb_arg(int argc, char* argv0)
{
  if (argc != 5) {
    printf(_PROGRAMME_);
	  printf(_PROGRAMME_USAGE, argv0);
    exit(1);
  }
}

void set_mode(char* argv1, char* argv0, int* mode)
{
  if (!strcmp(argv1,MODE_ENC))     
    *mode = 1;
  else if (!strcmp(argv1, MODE_DEC))
    *mode = 0;
  else 
  {
    printf(_PROGRAMME_);
	  printf(_PROGRAMME_USAGE, argv0);
	  exit(1);
  }
}

FILE* set_open_file_mode(FILE * file, const char * argv, const char * mode, const char * string)
{
  file = fopen(argv, mode);
  if (file == NULL) 
  {
  	fprintf(stderr, string, argv);
	  exit(1);
  } 
  return file;
}

void set_close_file_mode(FILE * file)
{
  if (file != NULL)
  {
    fclose(file);
  }
}
