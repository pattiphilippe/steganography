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

run : run_bmp run_gif

build : build_bmp build_gif

clean : clean_bmp clean_gif

run_bmp : build_bmp
	##################################################################################################
	# Run ReadBMP with "splash_multicolor_src.bmp" and "input_message.txt"                           #
	#                                                                                                #
	# ./dist/bmp/ReadBMP enc rsc/splash_color_src.bmp rsc/splash_color_src.bmp rsc/input_message.txt #
	##################################################################################################
	@echo
	@./dist/bmp/ReadBMP enc rsc/splash_color_src.bmp rsc/splash_color_src.bmp rsc/input_message.txt

build_bmp : dist/bmp/ReadBMP

dist/bmp/ReadBMP : dist/bmp/main.o dist/bmp/bitmap.o dist/bmp/utils.o
	gcc dist/bmp/main.o dist/bmp/bitmap.o dist/bmp/utils.o -o dist/bmp/ReadBMP

dist/bmp/main.o : src/bmp/main.c src/bmp/bitmap.h src/bmp/utils.h
	gcc -std=c99 -Wall -pedantic src/bmp/main.c -c -o dist/bmp/main.o

dist/bmp/bitmap.o : src/bmp/bitmap.c src/bmp/bitmap.h
	gcc -std=c99 -Wall -pedantic src/bmp/bitmap.c -c -o dist/bmp/bitmap.o

dist/bmp/utils.o: src/bmp/utils.c src/bmp/utils.h
	gcc -std=c99 -Wall -pedantic src/bmp/utils.c -c -o dist/bmp/utils.o

clean_bmp :
	@rm -f dist/bmp/*
	@rm -f rsc/*_dest.bmp




run_gif : build_gif
	###########################################################################
	# Run ReadGIF with "hacker.gif" as source file and "steg.gif as dest file #
	#                                                                         #
	# ./dist/gif/ReadGIF rsc/hacker.gif rsc/steg.gif                          #
	###########################################################################
	@echo
	@./dist/gif/ReadGIF rsc/hacker.gif rsc/steg.gif

build_gif : dist/gif/ReadGIF

dist/gif/ReadGIF : dist/gif/gif_util.o dist/gif/gif_steg.o dist/gif/gif_main.o 
	gcc -o dist/gif/ReadGIF dist/gif/gif_main.o dist/gif/gif_steg.o dist/gif/gif_util.o -lm 

dist/gif/gif_main.o : src/gif/gif_main.c src/gif/gif_steg.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/gif_main.o -c src/gif/gif_main.c   

dist/gif/gif_steg.o : src/gif/gif_steg.c src/gif/gif_steg.h src/gif/gif_util.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/gif_steg.o -c src/gif/gif_steg.c 

dist/gif/gif_util.o : src/gif/gif_util.c src/gif/gif_util.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/gif_util.o -c src/gif/gif_util.c 

clean_gif :
	@rm -f dist/gif/* rsc/steg.gif
