#NOM : Makefile
#CLASSE : SYSG5 - Steganography BMP
#OBJET : Demo du programme
#HOWTO : make ; make clean
#AUTEUR : Foud Hind et Patti Philippe

readBMP : ReadBMP
	@clear
	@./ReadBMP enc io/splash_multicolor_src.bmp io/splash_multicolor_dest.bmp io/input_message.txt

ReadBMP : main.o bitmap.o utils.o
	gcc main.o bitmap.o utils.o -o ReadBMP

main.o : main.c bitmap.h utils.h
	gcc -std=c99 -Wall -pedantic main.c -c -o main.o

bitmap.o : bitmap.c bitmap.h
	gcc -std=c99 -Wall -pedantic bitmap.c -c -o bitmap.o

utils.o: utils.c utils.h
	gcc -std=c99 -Wall -pedantic utils.c -c -o utils.o

clean :
	@rm -f ReadBMP *.o *~
	@rm -f io/*_dest.bmp

