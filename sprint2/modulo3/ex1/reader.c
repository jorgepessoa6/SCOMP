#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

typedef struct{
		int number;
		char name[20];
} student;
	
int main(void) {
	int fd, data_size = sizeof(student),r;
	student *stud;
	fd = shm_open("/ex1",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate (fd, data_size);
	stud = (student*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	printf("%d\n", stud->number);
    printf("%s\n", stud->name); 
    
    r = munmap(stud, data_size); /* disconnects */
	if (r < 0) exit(1); /* Check error */
	r = shm_unlink("/ex1"); /* removes */
	if (r < 0) exit(1); /* Check error */
    
    return 0;
}

		
