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

typedef struct
{
    char str[50][80];
    int writen;
} sharedDataType;

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

int main()
{
	
	char *SHARED_FILE = "/pl4_ex4";
	const int SIZE = sizeof(sharedDataType);
    sharedDataType *sharedData;

    // 1. Creates and opens a shared memory area
    int fd = shm_open(SHARED_FILE, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }
    
    // 2. Defines the size
    ftruncate(fd, SIZE);
    
    // 3. Get a pointer to the data
    sharedData = (sharedDataType *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sharedData->writen = 0;

    sem_t *sem;
	
	// Creates and opens a semaphore
    if ((sem = sem_open("/sem_ex4", O_CREAT, 0644, 1)) == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    int id = spawnChilds(50);
    
	int num = 0;
	
    if (id > 0){
        printf("\nDelete a string? (1 - Yes, 0 - No)\n");
	fflush(stdin);
	scanf("%d", &num); 
	
    if(num==1){
			struct timespec ts;
			if (clock_gettime(CLOCK_REALTIME, &ts) == -1){
				printf("clock_gettime failed");
				exit(EXIT_FAILURE);
			}
			
			ts.tv_sec += 12; // maximum time to access shm
			
			int s;
			
			s = sem_timedwait(sem, &ts);
			
			// Check if shm opened in time 

			if (s == -1)
			{
				if (errno == ETIMEDOUT)
				{
					printf("shm timed out\n");
					exit(EXIT_FAILURE);
				}
				else
				{
					perror("sem_timedwait");
					exit(EXIT_FAILURE);
				}
			}else {
				printf("shm opened with success\n");
				
			}
		}else{
			sem_wait(sem);
			printf("writing\n");
			sprintf(sharedData->str[sharedData->writen], "i am the father with id %d", id);
			sharedData->writen++;
			sleep(1);
			sem_post(sem);
		}
               
        
        // Undo mapping
        if (munmap(sharedData, SIZE) == -1)
        {
            perror("munmap failed");
            exit(EXIT_FAILURE);
        }
        // Close file descriptor
        if (close(fd) == -1)
        {
            perror("close failed");
            exit(EXIT_FAILURE);
        }
        // Close semaphore
        if (sem_close(sem) < 0)
        {
            perror("Error at sem close");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }
    
	

    int i;
    for (i = 0; i < 50; i++)
    {
        wait(NULL);
    }
    
    for (i = 0; i < 50; i++)
    {
        printf("%s\n", sharedData->str[i]);
    }
    
    // Undo mapping
    if (munmap(sharedData, SIZE) == -1)
    {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) == -1)
    {
        perror("close failed");
        exit(EXIT_FAILURE);
    }

    //so filho da unlink

    // Remove file from system
    if (shm_unlink(SHARED_FILE) < 0)
    {
        perror("Error at unlink share");
        exit(EXIT_FAILURE);
    }
    // Close semaphore
    if (sem_close(sem) < 0)
    {
        perror("Error at sem close");
        exit(EXIT_FAILURE);
    }
    // Unlink semaphote
    if (sem_unlink("/sem_ex03") < 0)
    {
        perror("Error at unlink sem");
        exit(EXIT_FAILURE);
    }

    return 0;
}
