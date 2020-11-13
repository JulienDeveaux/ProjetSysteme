CC=gcc
CXXFLAGS=-Wall -g -o2
LDFLAGS=-pthread 


projet: projet.o

projet.o: projet.c

clean:
	rm -rf *.o 