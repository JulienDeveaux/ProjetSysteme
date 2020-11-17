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




	if ((fork()&&fork()&&fork()) != 0) {
		close(fd[1]);

		while (read(fd[0], &valeur, sizeof(valeur)) != 0)
			RES[valeur]++;

		for(int i = 0; i < RANGE; i++) {
			printf("%d\n", RES[i]);
		}

		/*calcul de la variance :*/
		int tmp = 0;
		for(int i = 0; i < RANGE; i++) {						/*Ok y a un problème arrête de piquer le code internet et cherche un peu*/
			tmp += RES[i];
		}
		int moy = 0;
		moy = tmp / RANGE;
		int temp[RANGE];
		printf("moyenne : %d\n", moy);

		for(int i = 0; i < RANGE; i++) {
			temp[i] = RES[i] * RES[i];
		}
		int var = 0;
		for(int i = 0; i < RANGE; i++) {
			var += temp[i];
		}
		int res = 0;
		res = var / RANGE - moy * moy;
		printf("variance : %d\n", res);

		close(fd[0]);
	}
	else {
		close(fd[0]);
		srand(time(NULL));
		while(sem_wait(&lock) != 0) {
			printf("ATTENTE LOCK\n");
			wait(0);
		}
		for (int i = 0; i < NB; i++) {
			valeur = (rand() % RANGE);
			write(fd[1], &valeur, sizeof(valeur));
		}
		sem_post(&lock);
		close(fd[1]);

	}
	/*key_t keyTab;
	int idSHMTAB    = shmget(keyTab  ,sizeof(int)*(RAND_MAX), IPC_CREAT | IPC_EXCL | 0666 );
    int idSemaphore = semget( keyTab, 1, IPC_CREAT | IPC_EXCL | 0666 );
	//...
	char* pointeurSurTab = shmat(idSHMTAB, NULL, 0);
	//...
	int j = 0;
	//pointeurSurTab[j] += "unTruc";*/
	return 0;
}