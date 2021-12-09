#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define SHM_NAME "/shm4"
#define SIZE sizeof(char)*100

int main(void){
	
	int fd = shm_open(SHM_NAME, O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1) {
		printf("Erro a abrir Shared Memory.\n");
		return 0;
	}
	
	if(ftruncate(fd, SIZE) == -1) {
		printf("Erro a alocar espaço na Shared Memory.\n");
		return 0;
	}
	
	char *arr = (char*) mmap(NULL,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if(arr == NULL) {
		printf("Erro a mapear objeto.\n");
		return 0;
	}
	
	int i;
	printf("Caracteres no array.\n");
	for(i = 0; i < SIZE; i++) {
		printf("%d, ", arr[i]);
	}
	
	
	int average = 0;
	
	for (i = 0; i < SIZE; i++) {
		average += arr[i];
	}
	
	printf("\nMédia dos Caracteres do array: %d\n", average/100);
	
	
	if(munmap(arr, SIZE) == -1) {
		printf("Erro a desmapear objeto.\n");
		return 0;
	}
	
	int r;
	
	r = shm_unlink("/shm4"); /* removes */
	
	if (r < 0) exit(1); /* Check error */
	
	return 0;	
	
}
