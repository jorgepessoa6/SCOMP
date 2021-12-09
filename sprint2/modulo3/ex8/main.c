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
#define SHM_NAME "/shm8"

typedef struct{
	int num;
} Data_type;

int main(){

	
	int data_size = sizeof(Data_type), i;
	
	int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Failed shm_open\n");
		exit(EXIT_FAILURE);
	}
	
	if(ftruncate(fd, data_size) == -1){
		perror("Failed ftruncate\n");
		exit(EXIT_FAILURE);
	}
	
	Data_type* shared_data = (Data_type*) mmap(NULL, data_size, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0);
	if(shared_data == MAP_FAILED){
		perror("Failed mmap\n");
		exit(EXIT_FAILURE);
	}
	
	shared_data->num = 100;
	
	pid_t pid;
	
	if((pid = fork()) == -1){
		perror("Failed fork\n");
		exit(EXIT_FAILURE);
	}
	
	if(pid != 0){
		for(i = 0; i < size; i++){
			shared_data->num += 1;

			shared_data->num -= 1;
		}
		
		printf("Pai acabou com: %d\n", shared_data->num);
		
		wait(NULL);
		
	}else{
		for(i = 0; i < size; i++){
			shared_data->num += 1;
			
			shared_data->num -= 1;
		}
		
		printf("Filho acabou com: %d\n", shared_data->num);
		
		exit(0);
	}
	
	if(munmap(shared_data, data_size) == -1){
		perror("Failed munmap\n");
		exit(EXIT_FAILURE);
	}
	
	if(close(fd) == -1){
		perror("Failed close\n");
		exit(EXIT_FAILURE);
	}
	
	shm_unlink(SHM_NAME);
	
	return 0;
}
	
