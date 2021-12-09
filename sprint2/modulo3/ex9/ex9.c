#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#define ARRAY_SIZE 1000 
int main(void) {
	int vec[ARRAY_SIZE];
	int *data;
	time_t t;       	     /* needed to init. the random number generator (RNG) */ 
	int i,j,r; 
	/* intializes RNG (srand():stdlib.h; time(): time.h) */ 
	srand ((unsigned) time (&t));  
	/* initialize array with random numbers (rand(): stdlib.h) */ 
	for (i = 0; i < ARRAY_SIZE; i++){
		vec[i] = rand () % 1000;
	}
	
	
	int fd, data_size = sizeof(int)*10;
	fd = shm_open("/ex9", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate (fd, data_size);
	data = (int*) mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	
	pid_t p[10];
	int max=0;
	for (i = 0; i < 10; i++){
		p[i]=fork();
		if (p[i]==0){
			for (j = 0; j < 100; j++){
				if (vec[i*100+j]>max){
					max=vec[i*100+j];
					printf("novoMaximo:%d  %d\n",i,max);
				}
			}
			printf("MAXMAX:%d  %d\n",i,max);
			*(data+i)=max;
			exit(0);
		}
	}
	//Pai espera que todos os processos filho acabem
	int k;

	for (k = 0; k < 10; k++){
		wait(NULL);
	}
	int maxGlobal=0;
	
	for (i = 0; i < 10; i++){
		printf("Maximos Locais: %d\n",*(data+i));
		if (*(data+i)>maxGlobal){
			maxGlobal=*(data+i);
		}
	}
	printf("MaxGlobal %d\n:",maxGlobal);
	
	r = munmap(data, data_size); /* disconnects */
	if (r < 0) exit(1); /* Check error */
	r = shm_unlink("/ex9"); /* removes */
	if (r < 0) exit(1); /* Check error */
	
	return 0;
}
