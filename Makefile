CC=gcc
CXXFLAGS=-Wall -O3
LDFLAGS=-pthread

start: start.o

start.o: start.c

ProjetIPC: ProjetIPC.o

ProjetIPC.o: ProjetIPC.c

ProjetMieux: ProjetMieux.o

ProjetMieux.o: ProjetMieux.c

projet: projet.o

projet.o: projet.c

clean:
	rm -rf *.o 