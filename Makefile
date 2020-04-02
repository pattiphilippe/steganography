#NOM : Makefile
#CLASSE : SYSG5 - Steganography BMP
#OBJET : Demo du programme
#HOWTO : make ; make clean
#AUTEUR : Foud Hind et Patti Philippe

readBMP : dist/bmp/ReadBMP
	@clear
	@./dist/bmp/ReadBMP enc rsc/splash_multicolor_src.bmp rsc/splash_multicolor_dest.bmp rsc/input_message.txt

dist/bmp/ReadBMP : dist/bmp/main.o dist/bmp/bitmap.o dist/bmp/utils.o
	gcc dist/bmp/main.o dist/bmp/bitmap.o dist/bmp/utils.o -o dist/bmp/ReadBMP

dist/bmp/main.o : src/bmp/main.c src/bmp/bitmap.h src/bmp/utils.h
	gcc -std=c99 -Wall -pedantic src/bmp/main.c -c -o dist/bmp/main.o

dist/bmp/bitmap.o : src/bmp/bitmap.c src/bmp/bitmap.h
	gcc -std=c99 -Wall -pedantic src/bmp/bitmap.c -c -o dist/bmp/bitmap.o

dist/bmp/utils.o: src/bmp/utils.c src/bmp/utils.h
	gcc -std=c99 -Wall -pedantic src/bmp/utils.c -c -o dist/bmp/utils.o

clean :
	@rm -f dist/bmp/*
	@rm -f rsc/*_dest.bmp

