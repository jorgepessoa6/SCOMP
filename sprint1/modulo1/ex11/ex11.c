#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#define ARRAY_SIZE 1000
int main () { 
	int numbers[ARRAY_SIZE]; 
	int n;         		     
	time_t t;       	   
	int i; 
	srand ((unsigned) time (&t));  
	for (i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand () % 10000;
		while(numbers[i]>255){
			numbers[i] = rand () % 10000;
		}
	}
	pid_t p[5];
	int j, status,k;
	int max=0;
	for (i = 0; i < 5; i++){
		p[i]=fork();
		if (p[i]==0){
			for (j = 0; j < 200; j++){
				if (numbers[200*i+j]>max){
					max=numbers[200*i+j];
				}
			}
			exit(max);
		}
	}
	int max_absolute=0;
	for (k = 0; k < 10; k++){
		wait(&status);
		int val=WEXITSTATUS(status);
		printf("VALOR:%d\n",val);
		if (val>max_absolute){
			max_absolute=val;
		}
	}
	printf("%d\n",max_absolute);
	pid_t p2;
	p2=fork();
	int result[1000];
	if (p2==0){
		for (i = 0; i < ARRAY_SIZE/2; i++){
			result[i]=((int) numbers[i]/max_absolute)*100 ;
			printf("--> %d\n",result[i]);
		}
	}else{
		for (i = ARRAY_SIZE/2; i < ARRAY_SIZE; i++){
			result[i]=((int) numbers[i]/max_absolute)*100 ;
			printf("--> %d\n",result[i]);
		}
	}
	return 0;
}
	
	
	
	
