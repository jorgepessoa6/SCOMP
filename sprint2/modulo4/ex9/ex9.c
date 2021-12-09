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


void buy_chips(){
	printf("Bought chips\n");
}
void buy_beer(){
	printf("Bought beer\n");
}
void eat_and_drink(){
	printf("ate and drank :) \n");
}

int main(){	
	sem_t *sem, *sem_barrier;
	// Creates and opens a semaphore
    if ((sem = sem_open("/sem_ex9", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }
    if ((sem_barrier = sem_open("/sem_ex9_barrier", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }
    pid_t pid;
    pid = fork();
    
    if (pid !=0){
		buy_chips();
		sem_post(sem_barrier);		
		sem_wait(sem);
		
		eat_and_drink();
		
	}else{
		buy_beer();
		sem_post(sem);		
		sem_wait(sem_barrier);
		eat_and_drink();
		
	}
	// Close semaphore
    if (sem_close(sem) < 0)
    {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
    if (sem_close(sem_barrier) < 0)
    {
        perror("Error at sem_barrier close");
        exit(EXIT_FAILURE);
    }
    return 0;
	
   } 
    
    
    
    
    
    
    
