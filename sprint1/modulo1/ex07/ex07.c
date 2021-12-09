#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#define ARRAY_SIZE 1000 
int main () { 
	int numbers[ARRAY_SIZE]; /* array to lookup */ 
	int n;         		     /* the number to find */
	time_t t;       	     /* needed to init. the random number generator (RNG) */ 
	int i; 
	/* intializes RNG (srand():stdlib.h; time(): time.h) */ 
	srand ((unsigned) time (&t));  
	/* initialize array with random numbers (rand(): stdlib.h) */ 
	for (i = 0; i < ARRAY_SIZE; i++)
		numbers[i] = rand () % 10000; 
	/* initialize n */ 
	n = rand () % 10000; 
	pid_t p=fork();
	int cont=0;
	int status;
	if (p==0){
		for (i = 0; i < ARRAY_SIZE/2; i++){
			if (n==numbers[i]){
				cont++;
			}
		}
		printf("cont %d\n", cont); 
		exit(cont);
	}else{
		for (i = ARRAY_SIZE/2; i < ARRAY_SIZE; i++){
			if (n==numbers[i]){
				cont++;
			}
		}
		wait(&status); 
	}
	if (WIFEXITED(status)) {
		printf("Pai: %d , Filho:%d\n", cont, WEXITSTATUS(status));
	}
	return 0;
 }
