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
#define NUM_MES_MAX 15
#define FATHER 0
#define SON 1

char *shm_names[1] = {"/shm_ex06"};

char *sem_names[NUM_SEMS] = {"/sem_ex06_Father", "/sem_ex06_Son"};
int sem_values[NUM_SEMS] = {0, 1};

typedef struct{
	int num_mes;
} SharedData;

int main(){
	int fd = shm_open(shm_names[0], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	if(fd == -1){
		perror("Erro no shm_open\n");
		exit(0);
	}
	
	int size = sizeof(SharedData);
	
	if(ftruncate(fd, size) == -1){
		perror("Erro no ftruncate\n");
		exit(0);
	}
	
	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if(sd == NULL){
		printf("Erro no mmap\n");
		exit(0);
	}
	
	sem_t* sems[NUM_SEMS];
	int i;
	
	for(i = 0; i < NUM_SEMS; i++){
		sems[i] = sem_open(sem_names[i], O_CREAT | O_EXCL, 0644, sem_values[i]);
	
		if(sems[i] == SEM_FAILED){
			printf("Erro a criar semaforo!\n");
			return 0;
		}
	}
	
	pid_t pid;
	
	if((pid = fork()) == -1){
		perror("Erro no fork\n");
		exit(0);
	}
	
	if(pid == 0) {
		while(1){
			sem_wait(sems[SON]);
			
			if(sd->num_mes >= NUM_MES_MAX)
				break;
			
			printf("I'm the child\n");
			
			sd->num_mes++;
			
			sem_post(sems[FATHER]);
		}
		
		exit(0);
	}
	
	while(1){
		sem_wait(sems[FATHER]);
		
		if(sd->num_mes >= NUM_MES_MAX)
			break;
		
		printf("I'm the father\n");
		
		sd->num_mes++;
		
		sem_post(sems[SON]);
	}
	
	for(i = 0; i < NUM_SEMS; i++)
		sem_unlink(sem_names[i]);
	
	munmap(sd, size);
	close(fd);
	shm_unlink("/shm_ex06");
	
	return 0;
}