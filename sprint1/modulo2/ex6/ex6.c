#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define ARRAY_SIZE 1000
int main(){
	int fd[2];
	int vec1[ARRAY_SIZE]; 
	int vec2[ARRAY_SIZE]; 
	if(pipe(fd) == -1){
		perror("Pipe failed");
	}
	pid_t p[5];
	int i,j,l,tmp=0;
	int sums[5];
	for (l = 0; l < ARRAY_SIZE; l++){
		vec1[l]=l/100;
		vec2[l]=l/100;
	}
	for (i = 0; i < 5; i++){
		p[i]=fork();
		if (p[i]==0){
			for (j = 0; j < 200; j++){
				tmp+= vec1[i*200+j]+vec2[i*200+j];
			}
			printf("Soma: %d\n",tmp);
			close(fd[0]);
			write(fd[1], &tmp, sizeof(tmp));
			close(fd[1]);
			exit(0);
		}
		read(fd[0], &sums[i], sizeof(sums[i]));
	}
	close(fd[1]);
	close(fd[0]);
	int k;
	for (k = 0; k < 5; k++){
		tmp+=sums[k];
	}
	printf("SomaFinal: %d\n",tmp);
	return 0;
}
	
