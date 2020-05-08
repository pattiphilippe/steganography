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




run_bmp : build_bmp
	@echo
	@./dist/bmp/stegBMP enc rsc/bmp/splash_color_src.bmp rsc/bmp/splash_color_dest.bmp rsc/bmp/input_message.txt

	@./dist/bmp/stegBMP dec rsc/bmp/splash_color_dest.bmp rsc/bmp/output_message.txt


build_bmp : dist/bmp/stegBMP

dist/bmp/stegBMP : dist/bmp/main.o dist/bmp/encode_bmp.o dist/bmp/decode_bmp.o dist/utils/utils.o 
	gcc dist/bmp/main.o dist/bmp/encode_bmp.o dist/bmp/decode_bmp.o dist/utils/utils.o -o dist/bmp/stegBMP 

dist/bmp/main.o : src/bmp/main.c src/bmp/encode_bmp.h src/bmp/decode_bmp.h src/utils/utils.h
	gcc -std=c99 -Wall -pedantic src/bmp/main.c -c -o dist/bmp/main.o

dist/bmp/encode_bmp.o : src/bmp/encode_bmp.h src/bmp/encode_bmp.c src/utils/utils.h
	gcc -std=c99 -Wall -pedantic src/bmp/encode_bmp.c -c -o dist/bmp/encode_bmp.o

dist/bmp/decode_bmp.o : src/bmp/decode_bmp.h src/bmp/decode_bmp.c src/utils/utils.h
	gcc -std=c99 -Wall -pedantic src/bmp/decode_bmp.c -c -o dist/bmp/decode_bmp.o

dist/utils/utils.o: src/utils/utils.c src/utils/utils.h
	gcc -std=c99 -Wall -pedantic src/utils/utils.c -c -o dist/utils/utils.o

clean_bmp :
	@rm -f dist/bmp/*
	@rm -f rsc/*_dest.bmp
	@rm -f rsc/output_*.txt




run_gif : build_gif
	@echo
	@./dist/gif/ReadGIF enc rsc/gif/dog.gif rsc/gif/dog_dest.gif rsc/gif/input_message.txt

	@./dist/gif/ReadGIF dec rsc/gif/dog_dest.gif rsc/gif/output_message_dog.txt 


build_gif : dist/gif/ReadGIF 

dist/gif/ReadGIF : dist/gif/gif.o dist/gif/steg.o dist/gif/utils.o dist/gif/main.o 
	gcc -o dist/gif/ReadGIF dist/gif/main.o dist/gif/steg.o dist/gif/utils.o  dist/gif/gif.o -lm 

dist/gif/main.o : src/gif/main.c src/gif/steg.h
	gcc -std=c99 src/gif/main.c -c -o dist/gif/main.o 

dist/gif/steg.o : src/gif/steg.c src/gif/steg.h src/gif/gif.h 
	gcc -std=c99 src/gif/steg.c -c -o dist/gif/steg.o 

dist/gif/utils.o : src/utils/utils.c src/utils/utils.h
	gcc -std=c99 src/utils/utils.c -c -o dist/gif/utils.o

dist/gif/gif.o : src/gif/gif.c src/gif/gif.h
	gcc -std=c99 src/gif/gif.c -c -o dist/gif/gif.o 

clean_gif :
	@rm -f dist/gif/* rsc/steg.gif rsc/read_gif.log
	@rm -f rsc/*_dest.gif
	@rm -f rsc/output_*.txt