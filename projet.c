#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>
#include <semaphore.h>
#define NB 10000
#define RANGE 10

void alea();
//int ACCES = 0;				/*COPIE FAITE A CHAQUE FORK INUTILISABLE TEL QUEL*/
int RES[9000];
//int ITER;
sem_t lock;
static int *glob_var;

int main(int argc, char const *argv[]) {
	glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	int *ptr = mmap ( NULL, NB* sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0 );

	*glob_var = 0;
	//ITER = 0;
	int tabTemp[NB];
	int *tabTab[10];
	int fd[2];
	pipe(fd);

	if(sem_init(&lock,1,1) != 0) {
		return -1;
	}
	//write(fd[2], &ACCES, sizeof(ACCES));
	if ((/*fork()&&fork()&&fork()&&fork()&&fork()&&fork()&&fork()&&*/fork()&&fork()) != 0) {
		close(fd[1]);
		munmap(glob_var, sizeof *glob_var);
		/*read(fd[2], &ACCES, sizeof(ACCES));
		printf("Clé d'accès avant while: %d\n", ACCES);
		while(ACCES =! 10){
			read(fd[2], &ACCES, sizeof(ACCES));
			printf("Clé d'accès : %d\n", ACCES);
			sleep(2);
		}
		printf("On continue\n");
		printf("Clé d'accès ? : %d\n", ACCES);*/
		while(*glob_var != 2){
			printf("while %d\n", *glob_var);
			munmap(glob_var, sizeof *glob_var);
		}
		printf("On continue\n");

		/*for(int i = 0; i < 10; i++) {
			tabTab[i] = (int*)read(fd[0], &tabTemp, sizeof(tabTemp));			/*Segmente*//*
		}*/

		munmap(ptr, sizeof *ptr);
		/*for(int i = 0; i < 10; i++) {
			for (int j = 0; j< 10; j++){
				printf("%d", *(tabTab[j] + i));									/*Affichage des tableaux a l'intérieur de tabTab mais pas sur des valeurs du for*//*
			}
		}*/
		/*Début du truc pour savoir si c'est équilibré sa compte le nombre d'occurence des valeur dans RES et sa les affiche*/
		int final[RANGE];
		for(int i = 0; i < 9000; i++) {
			final[RES[i]]++;
		}
		for(int i = 0; i < RANGE; i++) {
			printf("%d\n", final[i]);
		}

		close(fd[0]);
		close(fd[2]);
	} else {
		srand(time(NULL));
		
		printf("fini\n");
		//read(fd[2], &ACCES, sizeof(ACCES));
		//ACCES++;
		*glob_var++;
		while(sem_wait(&lock) != 0) {											/*Verouillage sémaphore*/
			printf("ATTENTE LOCK\n");
			wait(0);
		}
		printf("LOCK\n");
		//write(fd[2], &ACCES, sizeof(ACCES));
		//write(fd[1], &tabTemp, sizeof(tabTemp));
		for (int i = 0; i < NB; i++) {
			ptr[i] = (rand() % RANGE) + 1;
		}
		sem_post(&lock);														/*Déverouillage sémaphore*/
		printf("UNLOCK\n");
	}
	sleep(0);
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