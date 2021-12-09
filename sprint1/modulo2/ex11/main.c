#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CHILDS 5

int main(void) {
	int i, p, child, child_num, child_read_num, max_num;
	int fd[NUM_CHILDS + 1][2];
	
	time_t t;
	srand((unsigned) time(&t));
	
	for(i = 0; i < NUM_CHILDS + 1; i++){
		if(pipe(fd[i]) == -1) {
			printf("Erro no pipe\n");
			return 0;
		}
	}
			
	for(i = 0; i < NUM_CHILDS; i++) {
		if((child = fork()) == -1) {
			printf("Erro a criar processo filho");
			return 0;
		}
		
		if(child == 0) {
			for(p = 0; p <= i; p++)
				child_num = 1 + (rand() % 500); // Para gerar um numero aleatorio diferente em cada processo filho.
			
			close(fd[i][1]);
			read(fd[i][0], &child_read_num, sizeof(int));
			close(fd[i][0]);
		
			max_num = (child_num > child_read_num ? child_num : child_read_num);
			
			printf("Random number by PID %d [i = %d]: %d\n", getpid(), i, child_num);
			
			p = i + 1;			
			close(fd[p][0]);
			write(fd[p][1], &max_num, sizeof(int));
			close(fd[p][1]);
			
			exit(0);
		}
	}
	
	for(i = 0; i <= NUM_CHILDS; i++)
		child_num = 1 + (rand() % 500); // Para gerar um numero aleatorio diferente em cada processo filho.
	
	printf("Random number by PID %d: %d\n", getpid(), child_num);
	
	close(fd[0][0]);
	write(fd[0][1], &child_num, sizeof(int));
	close(fd[0][1]);
	
	close(fd[NUM_CHILDS][1]);
	read(fd[NUM_CHILDS][0], &child_read_num, sizeof(int));
	close(fd[NUM_CHILDS][0]);
	
	max_num = (child_num > child_read_num ? child_num : child_read_num);
	printf("Max num: %d\n", max_num);
	
	return 0;
}
