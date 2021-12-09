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


int main(){	
	sem_t *sem;
	// Creates and opens a semaphore
    if ((sem = sem_open("/sem_ex5", O_CREAT, 0644, 0)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }
    pid_t pid;
    pid = fork();
    if (pid ==0){
		printf("Im the child\n");	
		sem_post(sem);
		
	}else{
		sem_wait(sem);
		printf("Im the father\n");	
		
	}
    // Close semaphore
    if (sem_close(sem) < 0)
    {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
    return 0;
}
