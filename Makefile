CC=g++
CXXFLAGS=-Wall -g -o2

projet: projet.o

projet.o: projet.c

clean:
	rm -rf *.o 