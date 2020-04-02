#NOM : Makefile
#CLASSE : SYSG5 - Steganography Gif
#OBJET : Demo du programme
#HOWTO : make ; make run_gif; make build_gif; make clean
#AUTEUR : Foud Hind et Patti Philippe

run : run_gif

run_gif : build_gif
	###########################################################################
	# Run command with "hacker.gif" as source file and "steg.gif as dest file #
	#                                                                         #
	# ./dist/ReadGif rsc/hacker.gif rsc/steg.gif                              #
	###########################################################################
	@echo
	@./dist/ReadGif rsc/dog.gif rsc/steg.gif

build_gif : dist/ReadGif

dist/ReadGif : dist/gif_util.o dist/gif_steg.o dist/gif_main.o 
	gcc -o dist/ReadGif dist/gif_main.o dist/gif_steg.o dist/gif_util.o -lm 

dist/gif_main.o : src/gif/gif_main.c src/gif/gif_steg.h
	gcc -std=c99 -Wall -pedantic -o dist/gif_main.o -c src/gif/gif_main.c   

dist/gif_steg.o : src/gif/gif_steg.c src/gif/gif_steg.h src/gif/gif_util.h
	gcc -std=c99 -Wall -pedantic -o dist/gif_steg.o -c src/gif/gif_steg.c 

dist/gif_util.o : src/gif/gif_util.c src/gif/gif_util.h
	gcc -std=c99 -Wall -pedantic -o dist/gif_util.o -c src/gif/gif_util.c 

clean :
	rm -f ReadGif dist/* rsc/steg.gif
