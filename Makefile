#NOM : Makefile
#CLASSE : SYSG5 - Steganography BMP
#OBJET : Demo du programme
#HOWTO : make ; make clean
#AUTEUR : Foud Hind et Patti Philippe

readBMP : ReadBMP
	@clear
	@./ReadBMP enc hill_src.bmp hill_dest.bmp input_message.txt

ReadBMP : main.o test.o
	gcc main.o test.o -o ReadBMP

main.o : main.c test.h
	gcc -std=c99 -Wall -pedantic main.c -c -o main.o

test.o : test.c test.h
	gcc -std=c99 -Wall -pedantic test.c -c -o test.o

clean :
	@rm -f ReadBMP *.o *~

