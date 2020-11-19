#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#define NB 666667
#define RANGE 10

sem_t lock;
int RES[RANGE];
int valeur;

int main(int argc, char const *argv[])
{
	if(sem_init(&lock,1,1) != 0)
	{
		printf("fork failed\n");
		return -1;
	}
	int fd[2];
	if(pipe(fd) == -1)
	{
		printf("pipe failed\n");
		return -1;
	}


	if ((/*fork()&&fork()&&*/fork()&&fork()&&fork()) != 0)
	{
		close(fd[1]);

		while (read(fd[0], &valeur, sizeof(valeur)) != 0)
			RES[valeur]++;

		for(int i = 0; i < RANGE; i++) {
			printf("Case %d : %d\n", i+1, RES[i]);
		}

		/*calcul de l'espérance :*/
		int temp, moy, esp;
		temp = 0;
		moy = 0;
		esp = 0;
		for (int i = 0; i < RANGE; i++) {
			temp += RES[i];
		}
		printf("\033[1;32m");
		printf("Nombre de valeurs : %d\n", temp);
		printf("\033[0m");
		moy = temp / RANGE;
		printf("\033[1;33m");
		printf("Moyenne = %d\n", moy);
		printf("\033[0m");

		temp = 0;
		for (int i = 0; i < RANGE; i++) {
			temp += (RES[i] - moy) * (RES[i] - moy); 
		}
		//printf("temp = %d\n", temp);
		esp = temp / RANGE;
		printf("\033[1;36m");
		printf("Espérance = %d\n", esp);
		printf("\033[0m");
		close(fd[0]);
	}
	else
	{
		close(fd[0]);
		srand(time(NULL));
		while(sem_wait(&lock) != 0) {
			printf("ATTENTE LOCK\n");
			wait(0);
		}
		printf("\033[1;31m");
		printf("LOCK\n");
		printf("\033[0m");
		for (int i = 0; i < NB; i++) {
			valeur = (rand() % RANGE);
			write(fd[1], &valeur, sizeof(valeur));
		}
		sem_post(&lock);
		printf("\033[1;32m");
		printf("UNLOCK\n");
		printf("\033[0m");
		close(fd[1]);

	}
	return 0;
}