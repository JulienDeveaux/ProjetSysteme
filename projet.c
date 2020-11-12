#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <time.h>
#define NB 10000

void alea();
int ACCES = 0;				/*COPIE FAITE A CHAQUE FORK INUTILISABLE TEL QUEL*/
int RES[9000];
int ITER;

int main(int argc, char const *argv[]) {
	ITER = 0;
	int tabTemp[NB];
	int *tabTab[10];
	int fd[3];
	pipe(fd);				/*Plus de pipe ?https://bytefreaks.net/programming-2/c-programming-2/cc-pass-value-from-parent-to-child-after-fork-via-a-pipe*/

	if ((fork()&&fork()&&fork()&&fork()&&fork()&&fork()&&fork()&&fork()&&fork()) != 0) {
		close(fd[1]);
		read(fd[3], &ACCES, sizeof(ACCES));
		printf("Clé d'accès avant while: %d\n", ACCES);
		/*while(ACCES =! 10){													/*Problème étrange avec la condition d'entrée*//*
			read(fd[3], &ACCES, sizeof(ACCES));
			printf("Clé d'accès : %d\n", ACCES);
			sleep(2);
		}*/
		printf("On continue\n");
		printf("Clé d'accès ? : %d\n", ACCES);

		for(int i = 0; i < 10; i++) {
			tabTab[i] = (int*)read(fd[0], &tabTemp, sizeof(tabTemp));			/*Segmente*/
		}
		for(int i = 0; i < 10; i++) {
			for (int j = 0; j< 10; j++){
				printf("%d", *(tabTab[j] + i));									/*Affichage des tableaux a l'intérieur de tabTab mais pas sur des valeurs du for*/
			}
		}
		wait(0);
		/*Faire ici les moyennes des différentes valeurs dans RES pour voir si équilibré*/

		close(fd[0]);
		close(fd[3]);
	} else {
		close(fd[0]);
		srand(time(NULL));
		for (int i = 0; i < NB; i++) {
			tabTemp[i] = (rand() % 100) + 1;
		}
		printf("fini\n");
		ACCES++;
		write(fd[3], &ACCES, sizeof(ACCES));
		write(fd[1], &tabTemp, sizeof(tabTemp));
		close(fd[1]);
		close(fd[3]);
	}
	return 0;
}

/*void alea() {																		/*Pas sur réutilisable*//*
	printf("Clé d'accès : %d\n", ACCES);
	srand(time(NULL));
	int tab[NB];
	for (int i = 0; i < NB; i++) {
		tab[i] = (rand() % 100) + 1;
	}
	while(ACCES != 1) {
		printf("Clé mauvaise");
		wait(0);
	}
	ACCES = 0;
	printf("ITER : %d\n", ITER);
	for(int i = 0; i < sizeof(tab); i++){
		RES[ITER] = tab[i];
		ITER++;
	}
	ACCES = 1;
}*/