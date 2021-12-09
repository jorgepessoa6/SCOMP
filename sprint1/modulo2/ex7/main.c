#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAY_SIZE 1000
#define NUM_CHILDS 5

int main(void) {
	int vec_1[ARRAY_SIZE], vec_2[ARRAY_SIZE], result[ARRAY_SIZE];
	time_t t; 	// needed to init, the random number generator (RNG)*/
	int i, p;
	int child_length = ARRAY_SIZE / NUM_CHILDS;
	
	int fd[NUM_CHILDS][2];

	/* intializes RNG */
	srand ((unsigned) time (&t));

	/* initialize array with random numbers */
	for (i = 0; i < ARRAY_SIZE; i++) {
		vec_1[i] = rand () % 10;
		vec_2[i] = rand () % 10;
	}
	
	for(i = 0; i < NUM_CHILDS; i++) {
		if(pipe(fd[i]) == -1) {
			printf("Nao foi possivel criar pipe no %d\n", i);
			exit(0);
		}
		
		if(fork() == 0) {
			close(fd[i][0]);
			
			int start = i * (ARRAY_SIZE / NUM_CHILDS);
			int end = (i + 1) * (ARRAY_SIZE / NUM_CHILDS);
			
			int child_result[child_length];
			int k = 0;
			
			for(p = start; p < end; p++) {
				child_result[k] = vec_1[p] + vec_2[p];
				k++;
			}
			
			if(write(fd[i][1], &child_result, child_length * sizeof(int)) == -1) {
				printf("Nao conseguiu escrever no pipe no %d\n", i);
				exit(0);
			}
			close(fd[i][1]);
			
			exit(0);
		}
	}
	
	for(i = 0; i < NUM_CHILDS; i++) {		
		close(fd[i][1]);
		
		int child_result[child_length];
		if(read(fd[i][0], &child_result, child_length * sizeof(int)) == -1) {
			printf("Nao conseguiu ler no pipe no %d\n", i);
			exit(0);
		}
		close(fd[i][0]);
		
		int start = i * (ARRAY_SIZE / NUM_CHILDS);
		int end = (i + 1) * (ARRAY_SIZE / NUM_CHILDS);
		int k = 0;
		
		for(p = start; p < end; p++) {
			result[p] = child_result[k];
			k++;
		}
	}
	
	printf("Amostra: \n");
	for(p = 0; p < NUM_CHILDS; p++) {
		for(i = 0; i < 5; i++) {
			int pos = p * child_length + i;
			printf("vec_1[%d]: %d | vec_2[%d]: %d | result[%d]: %d\n", pos, vec_1[pos], pos, vec_2[pos], pos, result[pos]);
		}
	}
	
	
	return 0;
}
