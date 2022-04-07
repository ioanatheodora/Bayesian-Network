CC = gcc
CFLAG = -Wall -Wextra
EXE = bnet
OBJ = bnet.o


build: bnet

%.o: %.c graph.h bayes.h 
	gcc -c -o $@ $< -Wall -Wextra

bnet: bnet.o
	gcc $^ -Wall -Wextra util.h bayes.h -o bnet

run:
	./bnet

.PHONY: clean

clean:
	rm -f *.o bnet bnet.out bnet.in