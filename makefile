#NOM : Makefile
#CLASSE : SYSG5 - Steganography Gif
#OBJET : Demo du programme
#HOWTO : make ; make clean
#AUTEUR : Foud Hind et Patti Philippe

readGif : ReadGif
	@clear
	################################################
	# Run command with "hacker.gif" as source file #
	################################################
	@echo
	@./ReadGif rsc/hacker.gif

ReadGif : gif.o main.o 
	gcc main.o gif.o -o ReadGif -lm

main.o : main.c gif.h
	gcc -std=c99 -Wall -pedantic main.c -c -o main.o 

gif.o : gif.c gif.h
	gcc -std=c99 -Wall -pedantic gif.c -c -o gif.o 

clean :
	@rm -f ReadGif *.o *~
