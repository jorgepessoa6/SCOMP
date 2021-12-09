#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h> 
#include <time.h>
#include <semaphore.h>

#define NUM_SEMS 2
#define SEM_C 0
#define SEM_S 1

char *sem_names[NUM_SEMS] = {"/sem_ex08_C", "/sem_ex08_S"};
int sem_values[NUM_SEMS] = {2, 2};

int main(){
	int i;
	sem_t *sem[NUM_SEMS];
	
	for(i = 0; i < NUM_SEMS; i++) {
		sem[i] = sem_open(sem_names[i], O_CREAT | O_EXCL, 0644, sem_values[i]);
		if(sem[i] == SEM_FAILED){
			printf("Erro no sem_open\n");
			return 0;
		}
	}
	
	pid_t pid;
	
	if((pid = fork()) == -1) {
		perror("Erro no fork\n");
		exit(0);
	}
	
	if(pid == 0){
		while(1){
			sem_wait(sem[SEM_C]);
			
			printf("C");
			fflush(0);
			
			sleep(1);
			
			int val;
			sem_getvalue(sem[SEM_S], &val);
			
			if(val == 1)
				sem_post(sem[SEM_S]);
			if(val == 0){
				sem_post(sem[SEM_S]);
				sem_post(sem[SEM_S]);
			}
		}
		
		exit(0);
	}
	
	while(1){
		sem_wait(sem[SEM_S]);
		
		printf("S");
		fflush(0);
		
		sleep(1);
		
		int val;
		sem_getvalue(sem[SEM_C], &val);
		
		if(val == 1)
			sem_post(sem[SEM_C]);
		if(val == 0){
			sem_post(sem[SEM_C]);
			sem_post(sem[SEM_C]);
		}
	}
	
	//O programa não chega ao unlink pois ele é interrompido com o ctrl Z 
	for(i = 0; i < NUM_SEMS; i++)
		sem_unlink(sem_names[i]);
		
	
	return 0;
}
