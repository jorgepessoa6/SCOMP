#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
	pid_t p = fork();
	
	if(p < 0) {
		printf("Erro a criar child process");
		exit(0);
	}
	
	if(p != 0) {
		printf("I'm...\n");
		wait(NULL);
		p = fork();
		if(p != 0) {
			printf("the...\n");
			wait(NULL);
			p = fork();
			if(p != 0)
				printf("father...\n");
			else
				printf("I'll never join you!\n");
		}
		else
			printf("I'll never join you!\n");
	}
	else
		printf("I'll never join you!\n");
	
	return 0;
}
