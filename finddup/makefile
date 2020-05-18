CFLAGS = -c -ansi -pedantic
COMPILER = gcc

fdup : MD5_calculator.o DirWalk.o H_Hash.o FindDuplicate.o
	$(COMPILER) -o fdup *.o -lcrypto -lssl

MD5_calculator.o : MD5_calculator.c MD5_calculator.h
	$(COMPILER) $(CFLAGS) MD5_calculator.c

DirWalk.o : DirWalk.c DirWalk.h
	$(COMPILER) $(CFLAGS) DirWalk.c

H_Hash.o : H_Hash.c H_Hash.h
	$(COMPILER) $(CFLAGS) H_Hash.c

FindDup.o : FindDuplicate.c FindDuplicate.h DirWalk.h H_Hash.h MD5_calculator.h
	$(COMPILER) $(CFLAGS) FindDup.c

.PHONY: clean
clean:
	rm -f *.o
