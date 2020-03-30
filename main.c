#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>

#include "gif.h"

void printBytes(const char* title, const unsigned char * bytes, size_t size);
bool hasGlobalColorTable(const header_lsd_t * header_lsd); 
void printBitsOfByte(const char* title, const unsigned char * byteSrc);
unsigned sizeOfGlobalColorTable(const header_lsd_t * header_lsd);

int main(int argc, char* argv[])
{
  printf("%s\n", "Start program\n");
  if(argc != 2){
	  printf("Please give one and only one arg : srcFilename!\n");
	  return -1;
  }
  
  FILE * gif_src = fopen(argv[1], "r");
  
  if(gif_src == NULL) {
	  perror("Open ");
	  return -1;
  }

  header_lsd_t header_lsd;
  
  fread(&header_lsd, sizeof(char), sizeof(header_lsd_t), gif_src);
 
  printBytes("Header", header_lsd.header, sizeof(header_lsd.header));
  printf("Logical Screen Descriptor\n");
  printBytes("Width", header_lsd.width, sizeof(header_lsd.width));
  printBytes("Height", header_lsd.height, sizeof(header_lsd.height));
  printBitsOfByte("PackedField ", &header_lsd.packed_field);
  bool hasGCT = hasGlobalColorTable(&header_lsd);
  printf("hasGlobalColorTable %d\n", hasGCT);
  unsigned sizeGCT = sizeOfGlobalColorTable(&header_lsd);
  printf("sizeOfGlobalColorTable : %d\n", sizeGCT);
  
  printf("ftell() before fseek : %ld\n", ftell(gif_src));
  fseek(gif_src,sizeGCT,SEEK_CUR);
  printf("ftell() after fseek : %ld\n", ftell(gif_src));

  printf("sizeof unsigned char [6] : %d\n", sizeof(unsigned char [6]));
  printf("sizeof bool : %d\n", sizeof(bool));
  printf("sizeof unsigned char * : %d\n", sizeof(unsigned char *));
  
  char * buffer;
  enum gif_section section;
  fread(&buffer, 2, 1, gif_src);
  printBytes("read 2 bytes for section : ", &buffer, 2);
  fseek(gif_src, -2, SEEK_CUR);
  section = read_gif_section(gif_src);
  printf("read gif_section : %d\n",section);
  fread(&buffer, 1, 1, gif_src);
  printBytes("Application Section  length 1: \n", &buffer, 1);
  fseek(gif_src, -1, SEEK_CUR);

  printf("ftell() before passing data section : %ld\n", ftell(gif_src));
  printf("passSection return = %d\n", passDataSubBlocks(gif_src));
  printf("ftell() after passing data section : %ld\n", ftell(gif_src));

}


void printBytes(const char * title, const unsigned char * bytes, size_t size){
	printf("%s\n",title);
	for(int i = 0; i < size; ++i){
		if(! (i%8) && i)
			printf("\n");
		printf("0x%0x ", bytes[i]);
	}
	printf("\n\n");
}

bool hasGlobalColorTable(const header_lsd_t * header_lsd){
	return header_lsd->packed_field & (1<<7);
}



void printBitsOfByte(const char* title, const unsigned char * byteSrc){
	unsigned char byte = *byteSrc;
	printf("%s\n", title);
	for(int i = 0; i < 8; i++){
		if(! (i%4) && i)
			printf(" ");
		printf("%d", (byte & 0x80) ? 1 : 0);
		byte <<= 1;
	}
	printf("\n\n");
}

unsigned sizeOfGlobalColorTable(const header_lsd_t * header_lsd){
	unsigned char color_resolution = (header_lsd->packed_field & 0x70)>>4;
	printf("color_resolution : %d\n", color_resolution);
	return 3 * pow(2.0, (color_resolution + 1));
}

