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
#define RANGE 1000000		/* max 1000000 				*/

int main (int argc, char **argv){
    int i;                        /*      variable de boucle      */
    key_t shmkey;                 /*      shared memory key       */
    int shmid;                    /*      shared memory id        */
    sem_t *sem;                   /*      sync semaphore          *//*shared */
    pid_t pid;                    /*      fork pid                */
    int *p;              		  /*      variable partagee       *//*shared */
    unsigned int n;               /*      bombre de fork          */
    unsigned int value;           /*      valeur semaphore        */

    /* initialise une variable partagee dans une shared memory */
    shmkey = ftok ("/dev/null", 5);       /* chemin valide et un chiffre */
    printf ("shmkey pour p = %d\n", shmkey);
    shmid = shmget (shmkey, sizeof(int)*RANGE, IPC_CREAT | 0666);
    if (shmid < 0){                           /* shared memory error check */
        perror ("shmget\n");
        exit (1);
    }

    p = (int *)shmat (shmid, NULL, 0);   /* raccord de p à la shared memory */
    printf ("p est en mémoire partagée.\n\n");

    /********************************************************/

    printf ("Combien de fork ?\n");
    printf ("Forks: ");
    scanf ("%u", &n);

    /* initialisation des semaphores pour les shared processes */
    sem = sem_open ("pSem", O_CREAT | O_EXCL, 0644, n); 
    /* nom du semaphore "pSem" */

    printf ("semaphore initialisé\n\n");


    /* fork des processes fils */
    for (i = 0; i < n; i++){
        pid = fork ();
        if (pid < 0) {
        /* check si il y a des erreurs */
            sem_unlink ("pSem");   
            sem_close(sem);  
            /* unlink evite que le semaphore exit en boucle */
            /* Si un crash arrive à l'execution             */
            printf ("Fork error.\n");
        }
        else if (pid == 0)
            break;                  /* processus fils */
    }


    /*******************************************************/
    /******************   PROCESSUS PARENT   ***************/
    /*******************************************************/
    if (pid != 0) {
        /* on attend que les processus fils terminent tous */
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

		/* calcul de la moyenne et de l'espérance */
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


        /* détachement shared memory */
        shmdt  (p);
        shmctl (shmid, IPC_RMID, 0);

        /* cleanup semaphores */
        sem_unlink ("pSem");   
        sem_close(sem);  
        /* unlink evite que le semaphore exit en boucle */
        /* Si un crash arrive à l'execution             */
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
        sem_wait (sem);           /* Verouille le semaphore */
        printf("\033[1;31m");
        printf ("  Processus(%d) dans la section critique.\n", pid);
        printf("\033[0m");
        for(i = 0; i < RANGE; i++) {
        	p[(int)valeurs[i]]++; /*On incrémente le tableau partagé a chaque occurence d'une valeur */
        }
        sem_post (sem);           /* Libère le semaphore */
        printf("\033[1;31m");
        printf("  Processus %d fini\n", pid);
        printf("\033[0m");
        exit (0);
    }
}