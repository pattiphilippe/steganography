#NOM : Makefile
#CLASSE : SYSG5 - Steganography Gif
#OBJET : Demo du programme
#HOWTO : make ; make run; make build; make clean
#AUTEUR : Foud Hind et Patti Philippe

run : build 
	###########################################################################
	# Run command with "hacker.gif" as source file and "steg.gif as dest file #
	#                                                                         #
	# ./dist/ReadGif rsc/hacker.gif rsc/steg.gif                              #
	###########################################################################
	@echo
	@./dist/ReadGif rsc/hacker.gif rsc/steg.gif

build : dist/ReadGif

dist/ReadGif : dist/gif_util.o dist/gif_steg.o dist/main.o 
	gcc -o dist/ReadGif dist/main.o dist/gif_steg.o dist/gif_util.o -lm 

dist/main.o : main.c gif_steg.h
	gcc -std=c99 -Wall -pedantic -o dist/main.o -c main.c   

dist/gif_steg.o : gif_steg.c gif_steg.h gif_util.h
	gcc -std=c99 -Wall -pedantic -o dist/gif_steg.o -c gif_steg.c 

dist/gif_util.o : gif_util.c gif_util.h
	gcc -std=c99 -Wall -pedantic -o dist/gif_util.o -c gif_util.c 

clean :
	rm -f ReadGif dist/*
