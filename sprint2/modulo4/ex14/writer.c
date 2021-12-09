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

#define NUM_SEMS 3
#define SEM_R 0
#define SEM_W 1
#define SEM_M 2
#define SIZE 100

typedef struct{
	int nr_readers;
}SharedData;

char *shm_names[2] = {"/shm_ex14_N", "/shm_ex14_C"};
char *sem_names[NUM_SEMS] = {"/sem_ex14_R", "/sem_ex14_W", "/sem_ex14_M"};
int sem_values[NUM_SEMS] = {1, 1, 1};

int main(){
	int fd_1 = shm_open(shm_names[0], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	int fd_2 = shm_open(shm_names[1], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	if(fd_1 == -1 || fd_2 == -1){
		perror("Erro no shm_open\n");
		exit(0);
	}
	
	int size = sizeof(SharedData);
	
	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_1, 0);
	char *str = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd_2, 0);
	
	
	if(ftruncate(fd_1, size) == -1){
		perror("Erro no ftruncate\n");
		exit(0);
	}
	if(ftruncate(fd_2, size) == -1){
		perror("Erro no ftruncate\n");
		exit(0);
	}
	
	if(sd == NULL || str == NULL){
		printf("Erro no mmap\n");
		exit(0);
	}
	
	int i;
	sem_t *sem[NUM_SEMS];
	
	for(i = 0; i < NUM_SEMS; i++) {
		sem[i] = sem_open(sem_names[i], O_CREAT, 0644, sem_values[i]);
		if(sem[i] == SEM_FAILED){
			printf("Erro no sem_open\n");
			return 0;
		}
	}
	
	sem_wait(sem[SEM_R]);
	sem_wait(sem[SEM_W]);
	
	time_t timeinfo;
	time(&timeinfo);
    char *time_str = ctime(&timeinfo);
	
	sprintf(str, "PID : %d Current Time: %s\n", getpid(), time_str);
	
	sem_post(sem[SEM_R]);
	
	printf("Number of writers: 1\n Number of readers: %d\n", sd->nr_readers);
	fflush(0);	
	
	sem_post(sem[SEM_W]);
	sem_post(sem[SEM_R]);
		
	munmap(sd, size);
	munmap(str, 100);
	
	close(fd_1);
	close(fd_2);
	
	return 0;
}