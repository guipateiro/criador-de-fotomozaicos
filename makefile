# GRR20197152 Guilherme Costa Pateiro
# Universidade Federal do Parana , programacao2 - ERE2 A
# ultima edicao; 17/11/2020 17:11
CFLAGS = -lm -Wall 

all:	pastilhas.o
	gcc fotomosaico.c pastilhas.o $(CFLAGS) -o mosaico

pastilhas.o:	pastilhas.c			pastilhas.h 
	gcc -c  pastilhas.c pastilhas.h -Wall

clear:
	rm -f *.o	
	rm -f *.gch	

purge:	clear
	rm -f mosaico