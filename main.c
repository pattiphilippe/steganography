#include <stdio.h>
#include <errno.h>

#include "gif.h"

int main(int argc, char* argv[])
{
  printf("%s\n", "Start program");
  if(argc != 2){
	  printf("Please give one and only one arg : srcFilename!\n");
	  return 0;
  }
  
  FILE * gifSrc = fopen(argv[1], "rb");
  errno = 0;
  
  if(gifSrc == NULL) {
	  printf("\n");
	  printf("File \"%s\" doesn't seem to exist. Please check filepath and spelling. --\n", argv[1]);
	  return 0;
  }

  header_lsd_t header_lsd;
  	
}
