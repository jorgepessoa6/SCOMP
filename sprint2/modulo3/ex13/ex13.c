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
		char path[100];
		char word[3];
		int number;
} info;

int main(void) {
	pid_t p[10];
	info *information;
	
	int fd, data_size = sizeof(info)*10;
	fd = shm_open("/ex13", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate (fd, data_size);
	information = (info*) mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	int i,r;
	for (i = 0; i < 10; i++){
		sprintf(information[i].path,"/media/partilha/scomp_2df_g30/sprint2/modulo3/ex13/processo%d.txt",i);
		strcpy(information[i].word,"ola");
		information[i].number=0;
	}
	for (i = 0; i <10 ; i++){
		p[i]=fork();
		if (p[i]==0){
			
				char buff[20];
				char directory[65];
				sprintf(directory,"/media/partilha/scomp_2df_g30/sprint2/modulo3/ex13/processo%d.txt",i);
				FILE *fp=fopen( directory, "r");
				fscanf(fp, "%s", buff);
				char *buffHelp=&buff[0];
				int length = strlen(information[i].word);
				for (buffHelp = strstr(buffHelp, information[i].word); buffHelp;	 buffHelp = strstr(buffHelp + length, information[i].word))
					information[i].number++;	
				exit(0);
		}
		
	}
	int k;
	for (k = 0; k < 10; k++){
		wait(NULL);
	}
	for (i = 0; i < 10; i++)
	{
	printf("Numero de Ocorrencias[%d]: %d\n",i,information[i].number); 	
	}
	r = munmap(information, data_size); /* disconnects */
	if (r < 0) exit(1); /* Check error */
	r = shm_unlink("/ex13"); /* removes */
	if (r < 0) exit(1); /* Check error */
	return 0;
}
	
		
	
	
