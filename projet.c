#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <time.h>
#define NB 10000

void alea();
void newprocess();
int ACCES;

int main(int argc, char const *argv[]) {
	ACCES = 1;
	int res[90000];
	newprocess();
	return 0;
}


void newprocess() {
    pid_t pid = fork();           /* On duplique le processus père. */
    if (pid == -1) {              /* Erreur de duplication. */
        perror("fork");           /* Affiche l'erreur sur stderr. */
        exit(EXIT_FAILURE);       /* On quitte dans le processus père. */
    } else if (pid == 0) {        /* Si on est dans le processus fils. */
        printf("Dans le fils %d\n", getpid());
        alea();
        exit(EXIT_SUCCESS);
    } else {                      /* Si on est dans le processus père. */
        printf("Dans le père %d\n", pid);
        wait(NULL);
    }
}

void alea() {
	printf("Clé d'accès : %d\n", ACCES);
	srand(time(NULL));
	int tab[NB];
	for (int i = 0; i < NB; i++) {
		tab[i] = (rand() % 100) + 1;
	}
}