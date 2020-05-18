CFLAGS = -ansi -pedantic -g

aprog: HashMain.o H_Hash.o
	gcc -o Exe HashMain.o H_Hash.o

H_Hash.o : H_Hash.c H_Hash.h
	gcc -c $(CFLAGS) H_Hash.c

HashMain.o : HashMain.c H_Hash.h
	gcc -c $(CFLAGS) HashMain.c
