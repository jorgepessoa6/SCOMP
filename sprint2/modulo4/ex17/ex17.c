#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define num_semaforos 3

	typedef struct MovieTheater {
		int vipSpectator;
		int vipSpectatorWaiting;
		int specialSpectatorWaiting;
		int specialSpectator;
		int normalSpectator;
	} MovieTheater;
	
	typedef struct SharedDataStruct {
		MovieTheater MovieTheater;
	} SharedDataStruct;

int main(){
	
    char nome_semaforos[num_semaforos][50] = {"sem1", "sem2", "sem3"};
    int valor_semaforo[num_semaforos] = {1, 0, 0},i;
    sem_t *semaforos[num_semaforos];
	
	//Criação e definição dos semáforos
    for (i = 0; i < num_semaforos; i++) {
        if ((semaforos[i] = sem_open(nome_semaforos[i], O_CREAT | O_EXCL, 0644, valor_semaforo[i])) == SEM_FAILED) {
            perror("Error at sem_open()!\n");
            exit(1);
        }
    }
	
	//Memoria partilhada
	SharedDataStruct *sharedData;
	int size = sizeof(sharedData);
    int fd = shm_open("/ex17", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, size);
    sharedData = (SharedDataStruct *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sharedData->MovieTheater.normalSpectator = 0;
    sharedData->MovieTheater.specialSpectator = 0;
    sharedData->MovieTheater.vipSpectator = 0;
    sharedData->MovieTheater.specialSpectatorWaiting = 0;
    sharedData->MovieTheater.vipSpectatorWaiting = 0;
    
    //Cliente Normal
    for (i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error at fork()!");
            exit(1);
        }
		
        if (pid == 0) {
			
			printf("Normal Deseja MovieTheater\n");
	
			//Diminui valor do semáforo geral
			sem_wait(semaforos[0]);
			int specialSpectatorWaiting = sharedData->MovieTheater.specialSpectatorWaiting;
			
			int vipSpectatorWaiting = sharedData->MovieTheater.vipSpectatorWaiting;
			//Aumenta valor do semáforo geral
			sem_post(semaforos[0]);

			if (specialSpectatorWaiting > 0) {
				//Diminui valor do semáforo especial
				sem_wait(semaforos[2]);
			}

			if (vipSpectatorWaiting > 0) {
				//Decrementa uma unidade ao valor do semáforo vip
				sem_wait(semaforos[1]);
			}
			//Decrementa valor do semáforo geral
			sem_wait(semaforos[0]);
			printf("---->Normal entra\n");
			sharedData->MovieTheater.normalSpectator++;
			sleep(1);
			
			//Incrementa valor do semáforo geral
			sem_post(semaforos[0]);
			
			//Fecha semáforos
			for (i = 0; i < num_semaforos; i++) {
                if (sem_close(semaforos[i]) == -1) {
                    perror("Error at sem_close()!\n");
                    exit(1);
                }
            }
            exit(0);
        }
    }
    
    //Cliente Special
    for (i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error at fork()!");
            exit(1);
        }
		
        if (pid == 0) {
			printf("special Deseja MovieTheater\n");
	 
			//Decrementa uma unidade ao valor do semáforo geral
			sem_wait(semaforos[0]);
			sharedData->MovieTheater.specialSpectatorWaiting++;
			int vipSpectatorWaiting = sharedData->MovieTheater.vipSpectatorWaiting;
	 
	 
	 
			//Incrementa uma unidade ao valor do semáforo geral
			sem_post(semaforos[0]);
			if (vipSpectatorWaiting > 0) {
				//Decrementa uma unidade ao valor do semáforo vip
				sem_wait(semaforos[1]);
			}
	
	
	
			//Decrementa uma unidade ao valor do semáforo geral
			sem_wait(semaforos[0]);



			sharedData->MovieTheater.specialSpectatorWaiting--;
			int specialSpectatorWaiting = sharedData->MovieTheater.specialSpectatorWaiting;
	
	
	
			//Incrementa uma unidade ao valor do semáforo geral
			sem_post(semaforos[0]);


			if (specialSpectatorWaiting == 0) {
				//Incrementa uma unidade ao valor do semáforo special
				sem_post(semaforos[2]);
			}

			printf("------>Especial entra\n");
			sharedData->MovieTheater.specialSpectator++;
			sleep(1);	
			
			//Fecho de todos os semaforos
			for (i = 0; i < num_semaforos; i++) {
                if (sem_close(semaforos[i]) == -1) {
                    perror("Error at sem_close()!\n");
                    exit(1);
                }
            }
            exit(0);
        }
    }
    
    //Cliente Vip
    for (i = 0; i < 10; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error at fork()!");
            exit(1);
        }
		
		//Caso seja processo filho
        if (pid == 0) {
			printf("Vip Deseja MovieTheater\n");
	
			//Decrementa uma unidade ao valor do semáforo geral
			sem_wait(semaforos[0]);
			sharedData->MovieTheater.vipSpectatorWaiting++;
	
			//Incrementa uma unidade ao valor do semáforo geral
			sem_post(semaforos[0]);
	
			//Decrementa uma unidade ao valor do semáforo geral
			sem_wait(semaforos[0]);

			sharedData->MovieTheater.vipSpectatorWaiting--;
			int vipSpectatorWaiting = sharedData->MovieTheater.vipSpectatorWaiting;
    
			//Incrementa uma unidade ao valor do semáforo geral
			sem_post(semaforos[0]);

			if (vipSpectatorWaiting == 0) {
				//Incrementa uma unidade ao valor do semáforo vip
				sem_post(semaforos[1]);
			}

			printf("------>Vip entra\n");
			sharedData->MovieTheater.vipSpectator++;
			sleep(1);
			
			//Fecho de todos os semáforos
			for (i = 0; i < num_semaforos; i++) {
                if (sem_close(semaforos[i]) == -1) {
                    perror("Error at sem_close()!\n");
                    exit(1);
                }
            }
            exit(0);
        }
    }

    //Espera que todos os processos acabem
    for (i = 0; i < 300; i++) {
        wait(NULL);
    }
    
	//Fecha a zona de memoria partilhada
	munmap((void *)sharedData, size);
	close(fd);
    
	//Apaga a zona de memória partilhada
	shm_unlink("/ex17");

	//Fecha todos os semáforos (geral, special, vip)
	for (i = 0; i < num_semaforos; i++) {
		if (sem_close(semaforos[i]) == -1) {
			perror("Error at sem_close()!\n");
			exit(1);
		}
	}

    //Remove todos os semáforos do sistema (geral, special, vip)
    for (i = 0; i < num_semaforos; i++) {
        if (sem_unlink(nome_semaforos[i]) == -1) {
            perror("Error at sem_unlink()!\n");
            exit(1);
        }
    }
	return 0;
}


