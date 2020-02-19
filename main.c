#include <stdio.h>
#include <errno.h>

#include "gif.h"

void printBytes(const char* title, const char * string, size_t size);

int main(int argc, char* argv[])
{
  printf("%s\n", "Start program");
  if(argc != 2){
	  printf("Please give one and only one arg : srcFilename!\n");
	  return 0;
  }
  
  FILE * gif_src = fopen(argv[1], "rb");
  errno = 0;
  
  if(gif_src == NULL) {
	  printf("\n");
	  printf("File \"%s\" doesn't seem to exist. Please check filepath and spelling. --\n", argv[1]);
	  return 0;
  }

  header_lsd_t header_lsd;
  
  fread(&header_lsd, sizeof(char), sizeof(header_lsd_t), gif_src);
  printf("sizeof header_lsd %d\n", sizeof(header_lsd));
  printf("sizeof header, width, length : %d , %d, %d \n",sizeof(header_lsd.header), sizeof(header_lsd.width), sizeof(header_lsd.height));
  printf("sizeof char %d\n", sizeof(char));
  printf("sizeof unsigned char %d\n", sizeof(unsigned char));

 
  printBytes("Header", header_lsd.header, sizeof(header_lsd.header));
  printf("Logical Screen Descriptor\n");
  printBytes("Width", &header_lsd.width, sizeof(header_lsd.width));
  printBytes("Height", &header_lsd.height, sizeof(header_lsd.height));
}


void printBytes(const char * title, const char * string, size_t size){
	printf("%s\n",title);
	unsigned char *p = (unsigned char*) string;
	printf("strlen(string) = %d\n", strlen(string));
	printf("size = %d\n", size);
	for(int i = 0; i < size; ++i){
		if(! (i%8) && i)
			printf("\n");
		printf("0x%0x ", p[i]);
	}
	printf("\n\n");
}




