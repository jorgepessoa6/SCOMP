#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#define ARRAY_SIZE 1000 
int main(void) {
  
	const int LEITURA = 0;
    const int ESCRITA = 1;
	
	int vec[ARRAY_SIZE];
	time_t t;       	     /* needed to init. the random number generator (RNG) */ 
	int i,j;
	/* intializes RNG (srand():stdlib.h; time(): time.h) */ 
	srand ((unsigned) time (&t));  
	/* initialize array with random numbers (rand(): stdlib.h) */ 
	for (i = 0; i < ARRAY_SIZE; i++){
		vec[i] = rand () % 1000;
	}
	
	pid_t p[10];
	int max=0;

	int fd[10][2];
	
	// cria pipes
	for (i = 0; i < 10; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Pipe failed");
            return 1;
        }
    }
	
	for (i = 0; i < 10; i++){
		p[i]=fork();
		if (p[i]==0){
			close(fd[i][LEITURA]);
			for (j = 0; j < 100; j++){
				if (vec[i*100+j]>max){
					max=vec[i*100+j];
					printf("novoMaximo:%d  %d\n",i,max);
				}
			}
			printf("MAXMAX:%d  %d\n",i,max);
			write(fd[i][ESCRITA], &max, sizeof(int));
			close(fd[i][ESCRITA]);
			exit(0);
		}
	}
	
	int maxGlobal=0;
	int maxLocal=0;
	
	for (i = 0; i < 10; i++){
		if(p[i]>0){
		close(fd[i][ESCRITA]);
		read(fd[i][LEITURA], &maxLocal, sizeof(int));
		printf("Maximos Locais: %d\n", maxLocal);
		if (maxLocal>maxGlobal){
			maxGlobal=maxLocal;
		}
		close(fd[i][LEITURA]);
		}
		
	}
	printf("MaxGlobal %d\n:",maxGlobal);
	
	return 0;
}
