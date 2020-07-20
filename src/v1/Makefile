.PHONY: all
all: clpcreate clean

clpcreate: clpcreate.o clpconfin.o
	cc -I /usr/include/libxml2 -o clpcreate clpcreate.o clpconfin.o -lxml2
clpcreate.o: clpcreate.c
	cc -I /usr/include/libxml2 -c clpcreate.c -lxml2
clpconfin.o: clpconfin.c
	cc -I /usr/include/libxml2 -c clpconfin.c -lxml2

clpcreate.o: clpcreate.h
clpconfin.o: clpcreate.h

.PHONY: clean
clean:
	rm -f clpcreate.o clpconfin.o
