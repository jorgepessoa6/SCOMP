#include <semaphore.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>	
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> 

struct ponte{
	int e_o_count;
	int o_e_count;
	sem_t *e_o;
	sem_t *o_e;
};

int main(){
	int opcao = 0;
	int fd;
	pid_t pid;
	struct ponte *ptr_ponte;
	fd = shm_open("/shm_ponte", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
      printf("Failed shm_open\n");	
	}
	if (ftruncate(fd, sizeof(struct ponte)) == -1){
	  printf("Failed trucating\n");
	}
	ptr_ponte= mmap(NULL, sizeof(struct ponte), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(ptr_ponte==MAP_FAILED){
	  printf("Failed map\n");
	}
	ptr_ponte->e_o = sem_open("e_o",O_CREAT | O_EXCL, 0644,1);
	ptr_ponte->o_e = sem_open("o_e",O_CREAT | O_EXCL, 0644,1);
	if(ptr_ponte->e_o==SEM_FAILED){
		sem_unlink("e_o");
		ptr_ponte->e_o = sem_open("e_o",O_CREAT | O_EXCL, 0644,1);
	}
	if(ptr_ponte->o_e==SEM_FAILED){
		sem_unlink("o_e");
		ptr_ponte->o_e = sem_open("o_e",O_CREAT | O_EXCL, 0644,1);
	}
	ptr_ponte->o_e_count=0;
	ptr_ponte->e_o_count=0;
	do{
		printf("Ponte Interface:\n\t1) Atravessar Carro Este para Oeste\n\t2) Atravessar Carro Oeste para Este\n\t3) Sair\nOpção: ");
		scanf("%d",&opcao);
		switch(opcao){
			case 1:
				if(sem_trywait(ptr_ponte->e_o)==0 || ptr_ponte->o_e_count==0){
					ptr_ponte->e_o_count++;
					sem_wait(ptr_ponte->o_e);										
					printf("Número de carros a atravessar de Este para Oeste: %d\n",ptr_ponte->e_o_count);
					pid=fork();
					if(pid==0){
						sleep(5);
						printf("\nCarro atravessou a ponte com sucesso de Este para Oeste!\n");
						ptr_ponte->e_o_count--;
						exit(0);
					}
					sem_post(ptr_ponte->e_o);
				}else{
					printf("\nPonte ocupada, espere 30 segundos\n");
				}
			break;
			case 2:
				if(sem_trywait(ptr_ponte->o_e)==0 || ptr_ponte->e_o_count==0){
					ptr_ponte->o_e_count++;
					sem_wait(ptr_ponte->e_o);
					printf("Número de carros a atravessar de Oeste para Este: %d\n",ptr_ponte->o_e_count);
					pid=fork();
					if(pid==0){
						sleep(5);
						printf("\nCarro atravessou a ponte com sucesso de Oeste para Este!\n");
						ptr_ponte->o_e_count--;
						exit(0);
					}
					sem_post(ptr_ponte->o_e);
				}else{
					printf("\nPonte ocupada, espere 30 segundos\n");
				}
			break;
		
			case 3:
			break;
			default:printf("ERRO: Opção inválida, tente novamente\n\n");
		}
	}while(opcao!=3);
	sem_unlink("e_o");
	sem_unlink("o_e");
	return 0;
}
