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

int main(){
	sem_t *s1 = sem_open("s1", O_CREAT | O_EXCL, 0644,1);
	sem_t *s2 = sem_open("s2", O_CREAT | O_EXCL, 0664,1);
	if(s1==SEM_FAILED){
		sem_unlink("s1");
		s1 = sem_open("s1",O_CREAT | O_EXCL, 0644,1);
	}
	if(s2==SEM_FAILED){
		sem_unlink("s2");
		s2 = sem_open("s2",O_CREAT | O_EXCL, 0644,1);
	}
	pid_t pid[8];
	int status[8],i,j;
	int numbers[200];
	FILE *f1=fopen("/media/partilha/scomp_2df_g30/sprint2/modulo4/ex1/Numbers.txt", "r");
	FILE *f5 = fopen("Output.txt","w");
	fclose(f5);
	if(f1==NULL){
		printf("ERROR writing to file\n");
		exit(1);
	}
	fclose(f1);
	for(i=0;i<8;i++){
		if((pid[i]=fork())==0){
			sem_wait(s1);
			FILE *f2 = fopen("Numbers.txt","r");
			for(j=0;j<200;j++){
				fscanf(f2,"%d",&numbers[j]);
			}
			fclose(f2);
			sem_post(s1);
			sem_wait(s2);
			FILE *f3 = fopen("Output.txt","a");
			for(j=0;j<200;j++){
				fprintf(f3,"%d\n",numbers[j]);
			}
			fclose(f3);
			sem_post(s2);
			exit(0);
		}
	}	
	for(i=0;i<8;i++){
		wait(&status[i]);
	}
	FILE *f4 = fopen("Output.txt","r");
	printf("Contents do ficheiro:\n");
	for(i=0;i<1600;i++){
		int number;
		fscanf(f4,"%d",&number);
		printf("%d\n",number);
	}
	fclose(f4);
	sem_unlink("s1");
	sem_unlink("s2");
	return 0;
}
