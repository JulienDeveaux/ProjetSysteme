#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free(), random() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */
#include <time.h>			/* time()					*/
#include <unistd.h>			/* fork()					*/
#include <sys/wait.h>		/* waitpid()				*/
#define RANGE 10

int main (int argc, char **argv){
    int i;                        /*      loop variables          */
    key_t shmkey;                 /*      shared memory key       */
    int shmid;                    /*      shared memory id        */
    sem_t *sem;                   /*      synch semaphore         *//*shared */
    pid_t pid;                    /*      fork pid                */
    int *p;              		  /*      shared variable         *//*shared */
    unsigned int n;               /*      fork count              */
    unsigned int value;           /*      semaphore value         */

    /* initialize a shared variable in shared memory */
    shmkey = ftok ("/dev/null", 5);       /* valid directory name and a number */
    printf ("shmkey pour p = %d\n", shmkey);
    shmid = shmget (shmkey, sizeof(int)*RANGE, IPC_CREAT | 0666);
    if (shmid < 0){                           /* shared memory error check */
        perror ("shmget\n");
        exit (1);
    }

    p = (int *)shmat (shmid, NULL, 0);   /* attach p to shared memory */
    printf ("p=%d est en mémoire partagée.\n\n", *p);

    /********************************************************/

    printf ("Combien de fork?\n");
    printf ("Forks: ");
    scanf ("%u", &n);

    printf ("Quelle valeur Semaphore ?\n");
    printf ("Valeur Semaphore: ");
    scanf ("%u", &value);

    /* initialize semaphores for shared processes */
    sem = sem_open ("pSem", O_CREAT | O_EXCL, 0644, value); 
    /* name of semaphore is "pSem", semaphore is reached using this name */

    printf ("semaphores initialisé(s)\n\n");


    /* fork child processes */
    for (i = 0; i < n; i++){
        pid = fork ();
        if (pid < 0) {
        /* check for error      */
            sem_unlink ("pSem");   
            sem_close(sem);  
            /* unlink prevents the semaphore existing forever */
            /* if a crash occurs during the execution         */
            printf ("Fork error.\n");
        }
        else if (pid == 0)
            break;                  /* child processes */
    }


    /******************************************************/
    /******************   PARENT PROCESS   ****************/
    /******************************************************/
    if (pid != 0){
        /* wait for all children to exit */
        while (pid = waitpid (-1, NULL, 0)){
            if (errno == ECHILD)
                break;
        }

        printf ("\nParent: Tous les enfants sont finis.\n");

		/*calcul de l'espérance :*/
		int temp, moy, esp;
		temp = 0;
		moy = 0;
		esp = 0;
		for (int i = 0; i < RANGE; i++) {
			temp += p[i];
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
			temp += (p[i] - moy) * (p[i] - moy); 
		}
		//printf("temp = %d\n", temp);
		esp = temp / RANGE;
		printf("\033[1;36m");
		printf("Espérance = %d\n", esp);
		printf("\033[0m");


        /* shared memory detach */
        shmdt (p);
        shmctl (shmid, IPC_RMID, 0);

        /* cleanup semaphores */
        sem_unlink ("pSem");   
        sem_close(sem);  
        /* unlink prevents the semaphore existing forever */
        /* if a crash occurs during the execution         */
        exit (0);
    }

    /******************************************************/
    /******************   CHILD PROCESS   *****************/
    /******************************************************/
    else{
        srand(time(NULL));
        int valeurs[RANGE];
        for(int i = 0; i < RANGE; i++) {
			valeurs[i] = (rand() % RANGE);
			//printf("Valeur random %d : %d\n", i, valeurs[i]);
		}
        sem_wait (sem);           /* P operation */
        printf ("  Processus(%d) dans la section critique.\n", i);
        for(int i = 0; i < RANGE; i++) {
        	p[(int)valeurs[i]]++;
        }
        for(int i = 0; i < RANGE; i++) {
        	printf("Nombre d'occurence de %d : %d\n", i, p[i]);
        }
        sem_post (sem);           /* V operation */
        printf("  Processus %d fini\n", i);
        exit (0);
    }
}