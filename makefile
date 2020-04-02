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

run : run_gif #run_bmp 

build : build_gif #build_bmp

clean : clean_gif #clean_bmp 




run_bmp : build_bmp
	##################################################################################################
	# Run ReadBMP with "splash_multicolor_src.bmp" and "input_message.txt"                           #
	#                                                                                                #
	# ./dist/bmp/ReadBMP enc rsc/splash_color_src.bmp rsc/splash_color_dest.bmp rsc/input_message.txt #
	##################################################################################################
	@echo
	@./dist/bmp/ReadBMP enc rsc/splash_color_src.bmp rsc/splash_color_dest.bmp rsc/input_message.txt

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

dist/gif/ReadGIF : dist/gif/util.o dist/gif/steg.o dist/gif/main.o 
	gcc -o dist/gif/ReadGIF dist/gif/main.o dist/gif/steg.o dist/gif/util.o -lm 

dist/gif/main.o : src/gif/main.c src/gif/steg.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/main.o -c src/gif/main.c   

dist/gif/steg.o : src/gif/steg.c src/gif/steg.h src/gif/util.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/steg.o -c src/gif/steg.c 

dist/gif/util.o : src/gif/util.c src/gif/util.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/util.o -c src/gif/util.c 

clean_gif :
	@rm -f dist/gif/* rsc/steg.gif
