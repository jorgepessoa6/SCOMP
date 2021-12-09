#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

#define TIME_SHOW 3

int spawnChilds(int n)
{
    pid_t pid;
    int i;
    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}



int main() {
	
    sem_t *sem;


    // Create and open semaphores
    int i;
    if ((sem = sem_open("/a", O_CREAT | O_EXCL, 0644, 5)) == SEM_FAILED) {
        perror("Error at sem_open()!\n");
        exit(EXIT_FAILURE);
    }
    

    // Create all the children
    int id = spawnChilds(20);

    // All child process
    if (id != 0) {
		while(1){
			if(sem_trywait(sem)==0){
				printf("Visitor %d enters show \n", id);
				sleep(TIME_SHOW);
				sem_post(sem);
				printf("Visitor %d leaves show \n", id);				
				break;
			}else sleep(TIME_SHOW);
		}
		
        exit(EXIT_SUCCESS);
    }


    for (i = 0; i < 20; i++) {
        wait(NULL);
    }

    // Close all semaphores
    if (sem_close(sem) == -1) {
        perror("Error at sem_close()!\n");
        exit(EXIT_FAILURE);
    }

    // Remove semaphores from system
    if (sem_unlink("/a") == -1) {
		perror("Error at sem_unlink()!\n");
        printf("Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
	}    

    return EXIT_SUCCESS;
}
