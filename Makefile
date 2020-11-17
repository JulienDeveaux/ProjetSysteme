CC=gcc
CXXFLAGS=-Wall -g -o2
LDFLAGS=-pthread 

ProjetMieux: ProjetMieux.o

ProjetMieux.o: ProjetMieux.c

projet: projet.o

projet.o: projet.c

clean:
	rm -rf *.o 