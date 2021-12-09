#include <fcntl.h> /* For constants O_* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct{
	int num;
	char *str;
} sharedDataType;

int randomNumber(){
	int randomNumber = (rand() % 10) + 0;
	return randomNumber;
}

int main() {

    char *SHARED_FILE = "/ex03";
    const int NUMBER_OF_OPERATIONS = 100000;
	const int DATA_SIZE = sizeof(sharedDataType) * NUMBER_OF_OPERATIONS;

    // 1. Creates and opens a shared memory area
    int fd = shm_open(SHARED_FILE, O_EXCL | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // 2. Defines the size
    ftruncate(fd, DATA_SIZE);

    // 3. Get a pointer to the data
    sharedDataType *sharedData;
    sharedData = (sharedDataType *)mmap(NULL, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	// Initialize structures
	int i; 
	for (i = 0; i < NUMBER_OF_OPERATIONS; i++) {  
		sharedData->num = randomNumber(); 
		sharedData->str = "ISEP – SCOMP 2020";
		sharedData++;
	}
	    
    // Create a new process
    pid_t pid = fork();

    // If we are at the father - Pipes
    if (pid > 0) {
		printf("\nPipes Time: ");
    }

    // If we are at the child - Shared Memory
    if (pid == 0) {
		sharedDataType *array;
		
		// Start timer
		clock_t before = clock();
		
    	// Copy structures
    	int i;
    	for (i = 0; i < NUMBER_OF_OPERATIONS; i++) {
    		array = sharedData;
    		array++;
    		sharedData--;
    	}

       	//Stop timer
    	clock_t difference = clock() - before;

        printf("\nShared Memory Time: %d", (int)difference);
        
    }
    
	// Undo mapping
    if (munmap((void *)sharedData, DATA_SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }
    
    
    if(pid == 0){
        // Remove file from system
        if (shm_unlink(SHARED_FILE) < 0)
        {
            perror("Error at unlink");
            exit(EXIT_FAILURE);
        }
    }
    
	
    return EXIT_SUCCESS;

}
