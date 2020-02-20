#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#include "gif.h"

void printBytes(const char* title, const unsigned char * bytes, size_t size);
bool hasGlobalColorTable(const header_lsd_t * header_lsd); 
void printBitsOfByte(const char* title, const unsigned char * byteSrc);

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
  printf("sizeof header_lsd %ld\n", sizeof(header_lsd));
  printf("sizeof header, width, length, packed_field : %ld, %ld, %ld, %ld \n",sizeof(header_lsd.header), sizeof(header_lsd.width), sizeof(header_lsd.height), sizeof(header_lsd.packed_field));
  printf("sizeof char %ld\n", sizeof(char));
  printf("sizeof unsigned char %ld\n", sizeof(unsigned char));

 
  printBytes("Header", header_lsd.header, sizeof(header_lsd.header));
  printf("Logical Screen Descriptor\n");
  printBytes("Width", header_lsd.width, sizeof(header_lsd.width));
  printBytes("Height", header_lsd.height, sizeof(header_lsd.height));
  printBytes("PackedField", &header_lsd.packed_field, sizeof(header_lsd.packed_field));
  printf("hasGlobalColorTable %d\n", hasGlobalColorTable(&header_lsd));
  printf("hasGlobalColorTable %d\n", 128);
  printBitsOfByte("PackedField", &header_lsd.packed_field);
  printf("Test for printing binary value\n"); 
  printf("1 : %0x\n", 0b1);
  printf("2 : %0x\n", 0b10);
  printf("4 : %0x\n", 0b100);
  printf("8 : %0x\n", 0b1000);
  printf("16 : %0x\n", 0b10000);
  printf("32 : %0x\n", 0b100000);
  printf("64 : %0x\n", 0b1000000);
  printf("128 : %0x\n", 0b10000000);
  printf("255 : %0x\n", 0b11111111);
  printf("Is bit 2 from right at 1 :  !(4|0b011) == 0 : %d\n", !(4|0b011) == 0);
}


void printBytes(const char * title, const unsigned char * bytes, size_t size){
	printf("%s\n",title);
	printf("size = %ld\n", size);
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
	//if(! (i%4) && i)
		printf(" ");
		printf("%u", byte & 0x80 ? '1' : '0');
		byte <<= 1;
	}
	printf("\n\n");
}








