#include <stdio.h>
#include <errno.h>

#include "giff.h"

int main(int argc, char* argv[])
{
  printf("%s", "start program");
  
  FILE * giffSrc = fopen(argv[2], "rb");
  errno = 0;

  	
}
