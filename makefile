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

clean : clean_bmp clean_gif clean_utils




run_bmp : build_bmp
	@echo
	@./dist/bmp/stegBMP enc rsc/bmp/splash_color_src.bmp rsc/bmp/splash_color_dest.bmp rsc/bmp/input_message.txt
	@./dist/bmp/stegBMP enc rsc/bmp/hill_src.bmp rsc/bmp/hill_dest.bmp rsc/bmp/input_video.mp4

	@./dist/bmp/stegBMP dec rsc/bmp/splash_color_dest.bmp rsc/bmp/output_message.txt
	@./dist/bmp/stegBMP dec rsc/bmp/hill_dest.bmp rsc/bmp/output_video.mp4


build_bmp : dist/bmp/stegBMP

dist/bmp/stegBMP : dist/bmp/main.o dist/bmp/encode_bmp.o dist/bmp/decode_bmp.o dist/utils/general.o dist/utils/bmp.o
	gcc dist/bmp/main.o dist/bmp/encode_bmp.o dist/bmp/decode_bmp.o dist/utils/general.o dist/utils/bmp.o -o dist/bmp/stegBMP 

dist/bmp/main.o : src/bmp/main.c src/bmp/encode_bmp.h src/bmp/decode_bmp.h src/utils/general.h
	gcc -std=c99 -Wall -pedantic src/bmp/main.c -c -o dist/bmp/main.o

dist/bmp/encode_bmp.o : src/bmp/encode_bmp.h src/bmp/encode_bmp.c src/utils/general.h
	gcc -std=c99 -Wall -pedantic src/bmp/encode_bmp.c -c -o dist/bmp/encode_bmp.o

dist/bmp/decode_bmp.o : src/bmp/decode_bmp.h src/bmp/decode_bmp.c src/utils/general.h
	gcc -std=c99 -Wall -pedantic src/bmp/decode_bmp.c -c -o dist/bmp/decode_bmp.o


clean_bmp :
	@rm -f dist/bmp/*
	@rm -f rsc/bmp/*_dest.bmp
	@rm -f rsc/bmp/output_*




run_gif : build_gif
	@echo
	@./dist/gif/stegGIF enc rsc/gif/dog_src.gif rsc/gif/dog_dest.gif rsc/gif/input_message.txt
	@./dist/gif/stegGIF enc rsc/gif/murica_src.gif rsc/gif/murica_dest.gif rsc/gif/input_pdf.pdf

	@./dist/gif/stegGIF dec rsc/gif/dog_dest.gif rsc/gif/output_message.txt 
	@./dist/gif/stegGIF dec rsc/gif/murica_dest.gif rsc/gif/output_pdf.pdf


build_gif : dist/gif/stegGIF 

dist/gif/stegGIF : dist/gif/gif.o dist/gif/encode_gif.o dist/gif/decode_gif.o dist/utils/general.o dist/gif/main.o dist/utils/bmp.o 
	gcc -o dist/gif/stegGIF dist/gif/main.o dist/gif/encode_gif.o dist/gif/decode_gif.o dist/gif/gif.o dist/utils/general.o  dist/utils/bmp.o  -lm 

dist/gif/main.o : src/gif/main.c src/gif/encode_gif.h src/gif/decode_gif.h src/utils/general.h
	gcc -std=c99 src/gif/main.c -c -o dist/gif/main.o 

dist/gif/encode_gif.o : src/gif/encode_gif.c src/gif/encode_gif.h src/gif/gif.h src/utils/bmp.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/encode_gif.o -c src/gif/encode_gif.c

dist/gif/decode_gif.o : src/gif/decode_gif.c src/gif/decode_gif.h src/gif/gif.h src/utils/bmp.h
	gcc -std=c99 -Wall -pedantic -o dist/gif/decode_gif.o -c src/gif/decode_gif.c

dist/gif/gif.o : src/gif/gif.c src/gif/gif.h src/utils/bmp.h
	gcc -std=c99 src/gif/gif.c -c -o dist/gif/gif.o 

clean_gif :
	@rm -f dist/gif/* 
	@rm -f rsc/gif/*_dest.gif
	@rm -f rsc/gif/output_*



dist/utils/general.o: src/utils/general.c src/utils/general.h
	gcc -std=c99 -Wall -pedantic src/utils/general.c -c -o dist/utils/general.o

dist/utils/bmp.o: src/utils/bmp.c src/utils/bmp.h
	gcc -std=c99 -Wall -pedantic src/utils/bmp.c -c -o dist/utils/bmp.o

clean_utils : 
	@rm -f dist/utils/*