#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER 10


typedef struct{
	int buffer[10];
	int c;
	int nextProduced;
}buffer;

int main(){
	pid_t p;
	sem_t *sem;
	int fd,i, in=0, out=0;
	int nextConsumed;
	
	buffer * b;
	
	sem = sem_open("sem",	O_CREAT| O_EXCL, 0644, 1);
	if(sem == SEM_FAILED){
		perror("Failed sem_open");
		exit(0);
	}
	
	fd = shm_open("/ex13", O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Failed opening shared memory");
		exit(0);
	}
	
	if(ftruncate(fd,sizeof(buffer)) != 0){
		perror("Failed defining shared memory");
		exit(0);
	}
	
	b = (buffer*) mmap(NULL, sizeof(buffer), PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	b -> c = 0;
	b -> nextProduced = 0;
	
	for(i=0; i<2; i++){
		p = fork();
		if(p<0){
			perror("Fork Failed");
			exit(0);
		}
		if(p == 0){
			int next = 0;
			while(next < 15){
				
				while(b -> c == 10);
				b -> buffer[in] = b -> nextProduced;
				in = (in +1) % 10;
			
				sem_wait(sem);
				b -> c = b -> c+1;
				b -> nextProduced = b -> nextProduced +1;
				sem_post(sem);
			}
			exit(0);
		}
		
	}
	int value = 30;
	while(value>0){
		while(b-> c == 0);
		nextConsumed = b -> buffer[out];
		printf("Value: %d\n", nextConsumed);
		out = (out + 1) % 10;
			
		sem_wait(sem);
			
		b -> c = b -> c - 1;
			
		sem_post(sem);
			
		value--;
	}
	
	if(munmap(b, sizeof(buffer))<0){
		perror("Error munmap");
		exit(0);
	}
	
	if((shm_unlink("/ex13")) <0){
		perror("Error shm_unlink");
		exit(0);
	}
	
	if((sem_unlink("sem")) == -1){
		perror("Error sem_unlink");
		exit(0);
	}
	return 0;
	
}
	
			
	
