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

#define MY_SHARED_FILE "/ex07"
#define MY_SHM_OPEN_WRITER_OFLAG O_EXCL | O_RDWR | O_CREAT
#define MY_SHM_OPEN_WRITER_MODE S_IRUSR | S_IWUSR
#define MY_SHM_OPEN_READER_OFLAG O_EXCL | O_RDWR
#define MY_SHM_OPEN_READER_MODE S_IRUSR | S_IWUSR


int main() {

    const int ARRAY_SIZE = 10;
    const int SIZE = ARRAY_SIZE * sizeof(int);

    // 1. Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, MY_SHM_OPEN_READER_OFLAG, MY_SHM_OPEN_READER_MODE);
    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // 2. Defines the size
    ftruncate(fd, SIZE);

    // 3. Get a pointer to the data
    int *sharedData;
    sharedData = (int *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	


    // Calculate the average

    int total = 0;

    int i;
    for (i=0; i<ARRAY_SIZE; i++) {
        printf("%d\n", sharedData[i]);
        total = total + sharedData[i];
    }

    int average = total / ARRAY_SIZE;

    printf("Average: %d\n", average);


    // Undo mapping
    if (munmap((void *)sharedData, SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }

    // Remove file from system
    if (shm_unlink(MY_SHARED_FILE) < 0) {
        printf("Error at shm_unlink()!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
