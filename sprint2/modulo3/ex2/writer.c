#include "main.h"

int main() {

    // Creates and opens a shared memory area
    int fd = shm_open(MY_SHARED_FILE, MY_SHM_OPEN_WRITER_OFLAG, MY_SHM_OPEN_WRITER_MODE);

    if (fd == -1) {
        printf("Error at shm_open()!\n");
        exit(EXIT_FAILURE);
    }

    // Defines the size
    ftruncate(fd, MY_DATA_SIZE);

    // Get a pointer to the data
    Product *sharedData = mmap(NULL, MY_DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // Ask for input
    printf("Insert the product code:\n");
    scanf("%d", &sharedData->code);

    // Ask for input
    printf("Insert the product description:\n");
    scanf("%s", &sharedData->description);
    
    // Ask for input
    printf("Insert the product price:\n");
    scanf("%d", &sharedData->price);

    // Undo mapping
    if (munmap((void *)sharedData, MY_DATA_SIZE) < 0) {
        printf("Error at munmap()!\n");
        exit(EXIT_FAILURE);
    }

    // Close file descriptor
    if (close(fd) < 0) {
        printf("Error at close()!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
