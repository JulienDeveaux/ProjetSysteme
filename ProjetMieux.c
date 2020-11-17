#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#define NB 10000
#define RANGE 10

sem_t lock;
int RES[RANGE];
int valeur;

int main(int argc, char const *argv[])
{
	if(sem_init(&lock,1,1) != 0) {
		printf("fork failed\n");
		return -1;
	}
	int fd[2];
	if(pipe(fd) == -1) {
		printf("pipe failed\n");
		return -1;
	}


	if ((/*fork()&&fork()&&*/fork()&&fork()&&fork()) != 0) {
		close(fd[1]);

		while (read(fd[0], &valeur, sizeof(valeur)) != 0)
			RES[valeur]++;

		for(int i = 0; i < RANGE; i++) {
			printf("Case %d : %d\n", i, RES[i]);
		}

		/*calcul de l'espérance :*/
		int tmp[RANGE], moy, esp;
		tmp[1] = 0;
		for(int i = 0; i < RANGE; i++) {
			tmp[1] += RES[i];
		}
		moy = tmp[1] / RANGE;
		tmp[1] = 0;
		printf("Moyenne = %d\n", moy);

		for(int i = 0; i < RANGE; i++) {
			tmp[i] = RES[i] / 1000;
			printf("calcul : %d\n", (RES[i] / 1000));
		}

		for(int i = 0; i < RANGE; i++) {
			printf("tmp[%d] : %d\n", i, tmp[i]);
		}

		for(int i = 0; i < RANGE; i++) {
			esp += i * tmp[i];
		}
		printf("Espérance = %d\n", esp);
		close(fd[0]);
	}
	else {
		close(fd[0]);
		srand(time(NULL));
		while(sem_wait(&lock) != 0) {
			printf("ATTENTE LOCK\n");
			wait(0);
		}
		printf("\033[0;31m");
		printf("LOCK\n");
		printf("\033[0m");
		for (int i = 0; i < NB; i++) {
			valeur = (rand() % RANGE);
			write(fd[1], &valeur, sizeof(valeur));
		}
		sem_post(&lock);
		printf("\033[0;32m");
		printf("UNLOCK\n");
		printf("\033[0m");
		close(fd[1]);

	}
	return 0;
}