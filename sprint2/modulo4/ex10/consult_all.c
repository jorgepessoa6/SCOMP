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

#define NUM_SEMS 1
#define SIZE 80
#define NR_REC 100
#define SEM 0

typedef struct{
	int number;
	char name[SIZE];
	char address[SIZE];
}record;

typedef struct{
	record record[NR_REC];
	int index;
}SharedData;

char *shm_names[1] = {"/shm_ex10"};
char *sem_names[NUM_SEMS] = {"/sem_ex10"};
int sem_values[NUM_SEMS] = {1};

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
	
	int i;
	sem_t *sem[NUM_SEMS];
	
	for(i = 0; i < NUM_SEMS; i++) {
		sem[i] = sem_open(sem_names[i], O_CREAT, 0644, sem_values[i]);
		if(sem[i] == SEM_FAILED){
			printf("Erro no sem_open\n");
			return 0;
		}
	}
	
	sem_wait(sem[SEM]);
	
	if(sd->index == 0){
		printf("No records available.\n");
	}else{
		int i;
		for(i = 0; i < sd->index; i++){
			record *r = &(sd->record[i]);
			printf("Record:\nNumber: %d  Name: %s  Address: %s\n", r->number, r->name, r->address);
		}
	}
	
	sem_post(sem[SEM]);
	
	for(i = 0; i < NUM_SEMS; i++)
		sem_unlink(sem_names[i]);
	
	munmap(sd, size);
	close(fd);
	
	return 0;
}