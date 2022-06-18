CC=gcc
DEP=detectHypervisor.c
FLAGS=-I

all: collect lu suggest

collect: collect.c
	$(CC) -o collect collect.c $(DEP) $(FLAGS) .

suggest: suggest.c
	$(CC) -o suggest suggest.c $(DEP) $(FLAGS) .

detect: detect.c
	$(CC) -o detect detect.c $(DEP) $(FLAGS) .

clean:
	-rm *.o detect 