#NOM : Makefile
#CLASSE : SYSG5 
#OBJET : Steganographie avec Bitmap et GIF
#AUTEUR : Foud Hind et Patti Philippe
#HOWTO : make ; make run_gif; make build_gif; make clean

# make             : execute les demos
# make run         : execute les demos
# make build       : compile les demos	
# make clean       : supprime les fichiers generes	

# make run_bmp     : execute uniquement la demo pour le format bmp
# make build_bmp   : compile uniquement la demo pour le format bmp
# make clean_bmp   : supprime uniquement les fichiers generes lies au format bmp

# make run_gif     : execute uniquement la demo pour le format GIF
# make build_gif   : compile uniquement la demo pour le format GIF
# make clean_gif   : supprime uniquement les fichiers generes lies au format GIF

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
