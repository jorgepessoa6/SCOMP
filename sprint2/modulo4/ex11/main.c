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
#include <unistd.h>

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
	
    const char SEMAFOROS_NAME[4][80] = {"/passengers","/door1", "/door2", "/door3"};
    // first value for available capacity in the train
    const int SEMAFOROS_VALUE[4] = {50, 1, 1, 1};
    sem_t *semaforos[4];


    // Create and open semaphores
    int i;
    for (i = 0; i < 4; i++) {
        if ((semaforos[i] = sem_open(SEMAFOROS_NAME[i], O_CREAT | O_EXCL, 0644, SEMAFOROS_VALUE[i])) == SEM_FAILED) {
            perror("Error at sem_open()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Create all the children
    int id = spawnChilds(3);

    // Child process 1
    if (id == 1) {
		
		srand(time(NULL));
        int random = (rand() % 2) +1;
        
		// 1 -> passenger enters train
		// 2 -> passenger leaves train
        if (random == 1){
			sem_wait(semaforos[1]);	// block door
			sem_timedwait(semaforos[0], 1); // decrement available capacity, but doesnt wait for semaphore to be available
			sem_post(semaforos[1]); // unblock door
			printf("Entrou um passageiro na porta 1\n");
		} else{
			sem_wait(semaforos[1]); // block door
			int passengers;
			sem_getvalue(semaforos[0], &passengers);
			if(passengers==200){
				printf("Comboio ja esta vazio\n");
			}else{
				sem_post(semaforos[0]); // passenger leaves train
				printf("Saiu um passageiro na porta 1\n");
			}
			sem_post(semaforos[1]); // unblock door
		}
			
        exit(EXIT_SUCCESS);
    }

    // Child process 2
    if (id == 2) {
        srand(time(NULL));
        int random = (rand() % 2) +1;
        
		// 1 -> passenger enters train
		// 2 -> passenger leaves train
        if (random == 1){
			sem_wait(semaforos[2]);	// block door
			sem_timedwait(semaforos[0], 1); // decrement available capacity, but doesnt wait for semaphore to be available
			sem_post(semaforos[2]); // unblock door
			printf("Entrou um passageiro na porta 2\n");
		} else{
			sem_wait(semaforos[2]); // block door
			int passengers;
			sem_getvalue(semaforos[0], &passengers);
			if(passengers==200){
				printf("Comboio ja esta vazio\n");
			}else{
				sem_post(semaforos[0]); // passenger leaves train
				printf("Saiu um passageiro na porta 2\n");
			}
			sem_post(semaforos[2]); // unblock door
		}
			
        exit(EXIT_SUCCESS);
    }

    // Child process 3
    if (id == 3) {
        srand(time(NULL));
        int random = (rand() % 2) +1;
        
		// 1 -> passenger enters train
		// 2 -> passenger leaves train
        if (random == 1){
			sem_wait(semaforos[3]);	// block door
			sem_timedwait(semaforos[0], 1); // decrement available capacity, but doesnt wait for semaphore to be available
			sem_post(semaforos[3]); // unblock door
			printf("Entrou um passageiro na porta 3\n");
		} else{
			sem_wait(semaforos[3]); // block door
			int passengers;
			sem_getvalue(semaforos[0], &passengers);
			if(passengers==200){
				printf("Comboio ja esta vazio\n");
			}else{
				sem_post(semaforos[0]); // passenger leaves train
				printf("Saiu um passageiro na porta 3\n");
			}
			sem_post(semaforos[3]); // unblock door
		}
			
        exit(EXIT_SUCCESS);
    }

    for (i = 0; i < 3; i++) {
        wait(NULL);
    }

    // Close all semaphores
    for (i = 0; i < 4; i++) {
        if (sem_close(semaforos[i]) == -1) {
            perror("Error at sem_close()!\n");
            exit(EXIT_FAILURE);
        }
    }

    // Remove semaphores from system
    for (i = 0; i < 4; i++) {
        if (sem_unlink(SEMAFOROS_NAME[i]) == -1) {
            perror("Error at sem_unlink()!\n");
            printf("Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}
