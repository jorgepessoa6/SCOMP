#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define size 1000000
#define SHM_NAME "/shm6"

typedef struct{
	int num;
	char flag;
} Data_type;

int main(){
	int data_size = sizeof(Data_type), i;
	clock_t start, end;
	
	int fd_shm = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd_shm == -1){
		perror("Failed shm_open\n");
		exit(EXIT_FAILURE);
	}
	
	if(ftruncate(fd_shm, data_size) == -1){
		perror("Failed ftruncate\n");
		exit(EXIT_FAILURE);
	}
	
	Data_type* shared_data = (Data_type*) mmap(NULL, data_size, PROT_READ |PROT_WRITE, MAP_SHARED, fd_shm, 0);
	if(shared_data == MAP_FAILED){
		perror("Failed mmap\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid;
	
	if((pid = fork()) == -1){
		perror("Failed fork\n");
		exit(EXIT_FAILURE);
	}
	
	if(pid != 0){
		srand((unsigned) time(NULL));
		
		start = clock();
		
		shared_data->num = rand() %10;
		shared_data->flag = 1;
		
		for(i = 1; i < size; i++){
			while(shared_data->flag == 1);
			
			shared_data->num = rand() % 10;
			shared_data->flag = 1;
		}
		
		wait(NULL);
		
	}else{
		int num1;
		
		for(i = 0; i < size; i++){
			while(shared_data->flag == 0);
			
			num1 = shared_data->num;
			shared_data->flag = 0;
		}
		
		exit(0);
	}
	
	end = clock();
	
	float time_shm = (float) (end - start) / CLOCKS_PER_SEC;
	
	if(munmap(shared_data, data_size) == -1){
		perror("Failed munmap\n");
		exit(EXIT_FAILURE);
	}
	
	if(close(fd_shm) == -1){
		perror("Failed close\n");
		exit(EXIT_FAILURE);
	}
	
	shm_unlink(SHM_NAME);
	
	int fd_p[2];
	
	if(pipe(fd_p) == -1){
		perror("Failed pipe\n");
		exit(EXIT_FAILURE);
	}
	
	if((pid = fork()) == -1){
		perror("Failed fork\n");
		exit(EXIT_FAILURE);
	}
	
	if(pid != 0){
		close(fd_p[0]);
		
		start = clock();
		
		for(i = 0; i < size; i++){
			int num = rand() % 10;
			
			if(write(fd_p[1], &num, sizeof(int)) == -1){
				perror("Failed write\n");
				exit(EXIT_FAILURE);
			}
		}
		
		close(fd_p[1]);
		
		wait(NULL);
		
	}else{
		close(fd_p[1]);
		
		for(i = 0; i < size; i++){
			int res;
			
			if(read(fd_p[0], &res, sizeof(int)) == -1){
				perror("Failed read\n");
				exit(EXIT_FAILURE);
			}
		}
		
		close(fd_p[0]);
		
		exit(0);
	}
	
	end = clock();
	
	float time_p = (float) (end - start) / CLOCKS_PER_SEC;
	
	printf("Tempo total por memória partilhada: %f\nTempo total por pipes: %f\n", time_shm, time_p);
	
	return 0;
}
			