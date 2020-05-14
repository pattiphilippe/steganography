#NOM : Makefile
#CLASSE : SYSG5 - Steganography Rapport
#OBJET : Rapport du projet de stéganographie - Formats bitmap et GIF
#HOWTO : make ; make clean
#AUTEUR : Foud Hind et Patti Philippe

FN = rapport

$(FN): 	$(FN).pdf
	@evince	$(FN).pdf&
	
$(FN).pdf: $(FN).tex  intro.tex bitmap.tex gif.tex presentation.tex conclusion.tex

	@rm -f $(FN).pdf
	@latex $(FN).tex
	#Running latex twice to set the Table of contents directly
	@touch $(FN).pdf
	@latex $(FN).tex
	@rm -f $(FN).aux $(FN).log $(FN).out

	@dvips $(FN).dvi -o $(FN).ps

	@ps2pdf $(FN).ps
	@rm -f $(FN).ps


clean : 
	@rm -f *.aux *.log *.pdf *.toc *.ps 
