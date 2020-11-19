#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#define NB 1000
#define RANGE 10

int RES[RANGE];

/*sem_t *lock;
sem_t lock2;
int RES[RANGE];
int valeur[NB];

int main(int argc, char const *argv[])
{
	uid_t proj_id;
	key_t cle;
    int shmid;
    int *data[NB + 1];
    int status;

    //Création d'une clée unique avec l'iud du projet
    if ((getuid() & 0xff) != 0)
        proj_id =  getuid();
    else
        proj_id = -1;

    if((cle=ftok(".", proj_id)) == -1) {
    	perror("ftok");
    	return -1;
    }

    printf("Clef générée : 0x%04x\n", cle);
    if((shmid = shmget(cle, sizeof(int), 0644 | IPC_CREAT)) == -1) {
    	perror("shmget");
    	return -1;
    }

    *data = shmat(shmid, NULL, 0);
    if(data == (int *)(-1)) {
    	perror("shmat");
    	return -1;
    }
    if ((lock = malloc(sizeof(sem_t))) == NULL) {
    	perror("malloc");
    	return -1;
    }

    if(sem_init(lock,1,1) == -1 || sem_init(&lock2,1,1) == -1)
	{
		printf("fork failed\n");
		return -1;
	}




	if ((/*fork()&&fork()&&*//*fork()&&fork()&&fork()) != 0)
	{
		while(*data[NB+1] != 3)
			for(int i = 0; i <  NB; i++) {
				RES[*data[i]]++;
			}

		for(int i = 0; i < RANGE; i++) {
			printf("Case %d : %d\n", i+1, RES[i]);
		}

		//calcul de l'espérance :
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
		printf("temp = %d\n", temp);
		esp = temp / RANGE;
		printf("\033[1;36m");
		printf("Espérance = %d\n", esp);
		printf("\033[0m");
	}
	else
	{
		srand(time(NULL));
		for(int i = 0; i < NB; i++) {
			valeur[i] = (rand() % RANGE);
		}
		while(sem_wait(lock) == -1) {
			printf("ATTENTE LOCK\n");
			wait(0);
		}
		printf("\033[1;31m");
		printf("LOCK\n");
		printf("\033[0m");
		for (int i = 0; i < NB; i++) {
			//printf("for\n");
			*data[valeur[i]]++;
		}
		printf("ap for");
		data[NB+1]++;
		if(shmdt(data) == -1) {
			perror("shmdt");
			return -1;
		}
		if(sem_post(lock) == -1) {
			perror("sem_wait");
			return -1;
		}
		printf("\033[1;32m");
		printf("UNLOCK\n");
		printf("\033[0m");
	}
	return 0;
}*/



int main(int argc, char const *argv[])
{
	uid_t proj_id;
	key_t cle;
	int shmid;
	int *data[NB + 1];

	//Création d'une clée unique avec l'iud du projet
    if ((getuid() & 0xff) != 0)
        proj_id =  getuid();
    else
        proj_id = -1;

    if((cle=ftok(".", proj_id)) == -1) {
    	perror("ftok");
    	return -1;
    }

    printf("Clef générée : 0x%04x\n", cle);
    if((shmid = shmget(cle, sizeof(int), 0644 | IPC_CREAT)) == -1) {
    	perror("shmget");
    	return -1;
    }


    *data = shmat(shmid, NULL, 0);
    if(data == (int *)(-1)) {
    	perror("shmat");
    	return -1;
    }


    int sem = semget(cle, 1, IPC_CREAT|0666);
    // struct sembuf
    // {
    //     ushort_t sem_num;
    //     short sem_op;
    //     short sem_flg;
    // };

    int pid = fork();

    if(fork() && fork() == 0)
    {
        printf("child waiting for semaphore\n");
        struct sembuf s1;
        s1.sem_num = 0;
        s1.sem_op = 0;
        s1.sem_flg = 0;
        if(semop(id, &s1, 1) == -1)
            printf("fail");
        printf("child got lock\n");
        sleep(2);
    }
    else
    {
        printf("parent waiting for semaphore\n");
        printf("parent got lock\n");
        sleep(2);
        struct sembuf s1;
        s1.sem_num = 0;
        s1.sem_op = 1;
        s1.sem_flg = 0;
        semop(sem, &s1, 1);
        sem_destroy(&sem);
    }
	return 0;
}