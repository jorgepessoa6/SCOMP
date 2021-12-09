#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>

int spawnChilds(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}



int main() {
	
    const char SEMAFOROS_NAME[3][80] = {"/sem1", "/sem2", "/sem3"};
    const int SEMAFOROS_VALUE[3] = {1, 0, 0};
    sem_t *semaforos[3];


    // Create and open semaphores
    int i;
    for (i = 0; i < 3; i++) {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_VALUE[i])) == SEM_FAILED) {
            perror("Error at sem_open()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Create all the children
    int id = spawnChilds(3);

    // Child process 1
    if (id == 1) {
        sem_wait(semaforos[0]);
        fprintf(stderr,"Sistemas ");
        sem_post(semaforos[1]);

        sem_wait(semaforos[0]);
        fprintf(stderr,"a ");
        sem_post(semaforos[1]);

        exit(EXIT_SUCCESS);
    }

    // Child process 2
    if (id == 2) {
        sem_wait(semaforos[1]);
        fprintf(stderr,"de ");
        sem_post(semaforos[2]);

        sem_wait(semaforos[1]);
        fprintf(stderr,"melhor ");
        sem_post(semaforos[2]);

        exit(EXIT_SUCCESS);
    }

    // Child process 3
    if (id == 3) {
        sem_wait(semaforos[2]);
        fprintf(stderr,"Computadores - ");
        sem_post(semaforos[0]);

        sem_wait(semaforos[2]);
        fprintf(stderr,"disciplina!\n");
        exit(EXIT_SUCCESS);
    }

    for (i = 0; i < 3; i++) {
        wait(NULL);
    }

    // Close all semaphores
    for (i = 0; i < 3; i++) {
        if (sem_close(semaforos[i]) == -1) {
            perror("Error at sem_close()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Remove semaphores from system
    for (i = 0; i < 3; i++) {
        if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
            perror("Error at sem_unlink()!\n");
            printf("Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}
