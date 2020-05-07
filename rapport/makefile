#NOM : Makefile
#CLASSE : SYSG5 - Steganography Rapport
#OBJET : Rapport du projet de stéganographie - Formats bitmap et GIF
#HOWTO : make ; make clean
#AUTEUR : Foud Hind et Patti Philippe


rapport: 	rapport.pdf
	@evince	rapport.pdf&
	
rapport.pdf:	rapport.tex  intro.tex bitmap.tex gif.tex conclusion.tex
	@pdflatex rapport.tex

clean : 
	rm *.aux *.log *.pdf 
