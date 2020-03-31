#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "gif_steg.h"

void showUsage(const char *name);
void printBytesHexa(const char *title, const unsigned char *bytes, size_t size);
void printBitsOfByte(const char *title, const unsigned char *byteSrc);

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		errno = 22;
		perror("Wrong usage ");
		showUsage(argv[0]);
		exit(1);
	}

	printf("Max Local Color Tables in source file : %d\n", getMaxLCT(argv[1]));
}

void showUsage(const char *name)
{
	printf("Usage : %s SOURCE DESTINATION\n\n", name);
}

void printBytesHexa(const char *title, const unsigned char *bytes, size_t size)
{
	printf("%s\n", title);
	for (int i = 0; i < size; ++i)
	{
		if (!(i % 8) && i)
			printf("\n");
		printf("0x%02x ", bytes[i]);
	}
	printf("\n\n");
}

void printBitsOfByte(const char *title, const unsigned char *byteSrc)
{
	unsigned char byte = *byteSrc;
	printf("%s\n", title);
	for (int i = 0; i < 8; i++)
	{
		if (!(i % 4) && i)
			printf(" ");
		printf("%d", (byte & 0x80) ? 1 : 0);
		byte <<= 1;
	}
	printf("\n\n");
}
