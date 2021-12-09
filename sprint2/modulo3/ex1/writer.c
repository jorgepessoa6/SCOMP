#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

typedef struct{
		int number;
		char name[20];
} student;
	
int main(int argc, char *argv[]) {
	int fd, data_size = sizeof(student);
	student *stud;
	fd = shm_open("/ex1", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate (fd, data_size);
	stud = (student*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	printf("Nome \n");
	
	scanf("%s",stud->name);
	printf("Numero \n");
	int n;
	scanf("%d",&n);
	stud->number=n;
	
	return 0;
}

		
	
	
