#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define SHM_NAME "/shm4"
#define SIZE sizeof(char)*100

int main(void){
	
	int fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1) {
		printf("Erro a abrir Shared Memory.\n");
		return 0;
	}
	
	if(ftruncate(fd, SIZE) == -1) {
		printf("Erro a alocar espaço na Shared Memory.\n");
		return 0;
	}
	
	char *arr = (char*) mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(arr == NULL) {
		printf("Erro a mapear objeto.\n");
		return 0;
	}
	
	int i;
	srand( time(NULL) );
	for(i = 0; i < SIZE; i++) {
		arr[i] = rand() % 26 + 'A';
	}
	
	return 0;
		
		
}
