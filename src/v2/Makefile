.PHONY: all
all: clpcfset clean

clpcfset: clpcfset.o clpconfin.o
	cc -I /usr/include/libxml2 -o clpcfset clpcfset.o clpconfin.o -lxml2
clpcfset.o: clpcfset.c
	cc -I /usr/include/libxml2 -c clpcfset.c -lxml2
clpconfin.o: clpconfin.c
	cc -I /usr/include/libxml2 -c clpconfin.c -lxml2

clpcfset.o: clpcfset.h
clpconfin.o: clpcfset.h

.PHONY: clean
clean:
	rm -f clpcfset.o clpconfin.o
