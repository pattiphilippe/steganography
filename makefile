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

run : run_gif run_bmp

build : build_gif build_bmp

clean : clean_gif clean_bmp





#@./dist/bmp/ReadBMP enc rsc/splash_color_src.bmp rsc/splash_color_dest.bmp rsc/input_message.txt
run_bmp : build_bmp
	###################################################################################################
	# Run ReadBMP with "splash_multicolor_src.bmp" and "input_message.txt"                            #
	#                                                                                                 #
	# ./dist/bmp/ReadBMP enc rsc/splash_color_src.bmp rsc/splash_color_dest.bmp rsc/input_message.txt #
	###################################################################################################
	@echo
	@./dist/bmp/ReadBMP enc rsc/splash_color_src.bmp rsc/splash_color_dest.bmp rsc/input_message_splash.txt
	@./dist/bmp/ReadBMP enc rsc/hill_src.bmp rsc/hill_dest.bmp rsc/input_message_hill.txt 

	@./dist/bmp/ReadBMP dec rsc/splash_color_dest.bmp rsc/output_message_splash.txt
	@./dist/bmp/ReadBMP dec rsc/hill_dest.bmp rsc/output_message_hill.txt 

build_bmp : dist/bmp/ReadBMP

dist/bmp/ReadBMP : dist/bmp/main.o dist/bmp/bitmap.o dist/bmp/utils.o
	gcc dist/bmp/main.o dist/bmp/bitmap.o dist/bmp/utils.o -o dist/bmp/ReadBMP

dist/bmp/main.o : src/bmp/main.c src/bmp/bitmap.h src/utils/utils.h
	gcc -std=c99 -Wall -pedantic src/bmp/main.c -c -o dist/bmp/main.o

dist/bmp/bitmap.o : src/bmp/bitmap.c src/bmp/bitmap.h
	gcc -std=c99 -Wall -pedantic src/bmp/bitmap.c -c -o dist/bmp/bitmap.o

dist/bmp/utils.o: src/utils/utils.c src/utils/utils.h
	gcc -std=c99 -Wall -pedantic src/utils/utils.c -c -o dist/bmp/utils.o

clean_bmp :
	@rm -f dist/bmp/*
	@rm -f rsc/*_dest.bmp
	@rm -f rsc/output_*.txt




run_gif : build_gif
	###########################################################################
	# Run ReadGIF with "hacker.gif" as source file and "steg.gif as dest file #
	#                                                                         #
	# ./dist/gif/ReadGIF rsc/hacker.gif rsc/steg.gif                          #
	###########################################################################
	@echo
	@./dist/gif/ReadGIF enc rsc/murica_src.gif rsc/murica_dest.gif rsc/input_message_splash.txt
	#@./dist/gif/ReadGIF enc rsc/dog.gif rsc/dog_dest.gif rsc/input_message_hill.txt 

	@./dist/gif/ReadGIF dec rsc/murica_dest.gif rsc/output_message_murica.txt
	#@./dist/gif/ReadGIF dec rsc/dog_dest.gif rsc/output_message_dog.txt 

build_gif : dist/gif/ReadGIF 

dist/gif/ReadGIF : dist/gif/gif.o dist/gif/steg.o dist/gif/utils.o dist/gif/main.o 
	gcc -o dist/gif/ReadGIF dist/gif/main.o dist/gif/steg.o dist/gif/utils.o  dist/gif/gif.o -lm 

dist/gif/main.o : src/gif/main.c src/gif/steg.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/main.o -c src/gif/main.c   

dist/gif/steg.o : src/gif/steg.c src/gif/steg.h src/gif/gif.h 
	gcc -std=c99 -Wall -pedantic -o dist/gif/steg.o -c src/gif/steg.c 

dist/gif/utils.o : src/utils/utils.c src/utils/utils.h
	gcc -std=c99 -Wall -pedantic src/utils/utils.c -c -o dist/gif/utils.o

dist/gif/gif.o : src/gif/gif.c src/gif/gif.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/gif.o -c src/gif/gif.c 

clean_gif :
	@rm -f dist/gif/* rsc/steg.gif rsc/read_gif.log
	@rm -f rsc/*_dest.gif
	@rm -f rsc/output_*.txt