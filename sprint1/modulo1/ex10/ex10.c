#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#define ARRAY_SIZE 2000
int main () { 
	int numbers[ARRAY_SIZE]; 
	int n;         		     
	time_t t;       	   
	int i; 
	srand ((unsigned) time (&t));  
	for (i = 0; i < ARRAY_SIZE; i++)	
		numbers[i] = rand() % 10000; 
	n = rand () % 10000; 
	
	int k, j,status;
	pid_t p[10];
	for (i = 0; i < 10; i++){
		p[i]=fork();
		if (p[i]==0){		
			for (j = 0; j < 200; j++){
				if(numbers[200*i+j]==n){
					exit(j);
				}
			}
			exit(255);
		}
	}
	for (k = 0; k < 10; k++){
		wait(&status);
		printf("VALOR:%d\n",WEXITSTATUS(status));
	}
	return 0;
}	
	
	
