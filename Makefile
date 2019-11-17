ifeq ($(DEBUG), true)		
	CC = gcc -g
else
	CC = gcc
endif

all: main.o
	gcc -o testdir main.o

main.o: main.c
	gcc -c main.c

run:
	./testdir

clean:
	rm *.o
	rm testdir
	rm *~

memcheck:
	valgrind --leak-check=yes ./testrandfile