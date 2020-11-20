/* Projet réalisé en binome Julien Deveaux et Justine Lemesle */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#define RANGE 100    /* max 1000000 */

int main (int argc, char **argv){
    int i;
    key_t shmkey;
    int shmid;
    sem_t *sem;
    pid_t pid;
    int *p;
    unsigned int n;
    unsigned int value;

    /* initialise une variable partagee dans une shared memory */
    shmkey = ftok ("/dev/null", 5);
    printf ("shmkey pour p = %d\n", shmkey);
    shmid = shmget (shmkey, sizeof(int)*RANGE, IPC_CREAT | 0666);
    if (shmid < 0){
        perror ("shmget\n");
        exit (1);
    }

    p = (int *)shmat (shmid, NULL, 0);
    printf ("p est en mémoire partagée.\n\n");

    /********************************************************/

    printf ("Combien de fork ?\n");
    printf ("Forks: ");
    scanf ("%u", &n);

    /* initialisation des semaphores pour les shared processes */
    sem = sem_open ("pSem", O_CREAT | O_EXCL, 0644, n); 

    printf ("semaphore initialisé\n\n");


    /* fork des processes fils */
    for (i = 0; i < n; i++){
        pid = fork ();
        if (pid < 0) {
            sem_unlink ("pSem");   
            sem_close(sem);
            printf ("Fork error.\n");
        }
        else if (pid == 0)
            break;
    }


    /*******************************************************/
    /******************   PROCESSUS PARENT   ***************/
    /*******************************************************/
    if (pid != 0) {
        while (pid = waitpid (-1, NULL, 0)){
            if (errno == ECHILD)
                break;
        }

        printf ("\nParent: Tous les enfants sont finis.\n");
        printf("\033[1;32m");
        printf ("\nTableau final :\n");
        printf("\033[0m");
        for(i = 0; i < RANGE; i++) {
        	printf("Nombre d'occurence de %d : %d\n", i, p[i]);
        }
        int min, max;
        min = 0;
        max = 0;
        for(i = 0; i < RANGE; i++) {
        	if(p[i] < min) {
        		min = p[i];
        	} else if(p[i] > max) {
        		max = p[i];
        	}
        }
        printf("\033[1;35m");
        printf("Max : %d Min : %d\n", max, min);
        printf("\033[0m");

		long long unsigned int nbVal, moy, esp, temp;
		nbVal = 0;
		moy = 0;
		esp = 0;
		temp = 0;
		for (i = 0; i < RANGE; i++) {
			nbVal += p[i];
		}
		printf("\033[1;32m");
		printf("Nombre de valeurs : %lld\n", nbVal);
		printf("\033[0m");

		moy = nbVal / RANGE;

		printf("\033[1;33m");
		printf("Moyenne = %lld\n", moy);
		printf("\033[0m");

		for (i = 0; i < RANGE; i++) {
			temp += (p[i] - moy) * (p[i] - moy); 
		}
		esp = temp / RANGE;

		printf("\033[1;36m");
		printf("Espérance = %lld\n", esp);
		printf("\033[0m");


        shmdt  (p);
        shmctl (shmid, IPC_RMID, 0);

        sem_unlink ("pSem");   
        sem_close(sem);
        exit (0);
    }

    /*******************************************************/
    /******************   PROCESSUS FILS   *****************/
    /*******************************************************/
    else
    {
    	int pid = i;
        srand((unsigned) time(NULL) * getpid());
        int valeurs[RANGE];
        for(i = 0; i < RANGE; i++) {
			valeurs[i] = (rand() % RANGE);
		}
        sem_wait (sem);
        printf("\033[1;31m");
        printf ("  Processus(%d) dans la section critique.\n", pid);
        printf("\033[0m");
        for(i = 0; i < RANGE; i++) {
        	p[(int)valeurs[i]]++;
        }
        sem_post (sem);
        printf("\033[1;31m");
        printf("  Processus %d fini\n", pid);
        printf("\033[0m");
        exit (0);
    }
}