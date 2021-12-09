#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define INITIAL_CREDIT 20

int main(void) {
	time_t t;
	
	int balance = INITIAL_CREDIT;
	int fd[2][2];
	int i, keep_betting = 1;
	pid_t p;
	
	for(i = 0; i < 2; i++) {
		if(pipe(fd[i]) == -1) {
			printf("Erro a criar pipe!");
			return 0;
		}
	}
	
	p = fork();
	if(p == -1) {
		printf("Erro a criar processo filho!");
		return 0;
	} else if(p == 0) {
		srand ((unsigned) time (&t) - getpid());
		close(fd[0][1]);
		close(fd[1][0]);
		
		read(fd[0][0], &keep_betting, sizeof(int));
		
		while(keep_betting == 1) {
			int random = 1 + (rand() % 5);
			
			write(fd[1][1], &random, sizeof(int));
			read(fd[0][0], &balance, sizeof(balance));
			printf("Balance: %d\n", balance);
			read(fd[0][0], &keep_betting, sizeof(int));
		}
		
		close(fd[0][0]);
		close(fd[1][1]);
		exit(0);
	} else {
			srand ((unsigned) time (&t) - getpid());
		close(fd[0][0]);
		close(fd[1][1]);
		while(balance > 0) {
			int random = 1 + (rand() % 5);
			int bet_random_num;
			
			write(fd[0][1], &keep_betting, sizeof(int));
			read(fd[1][0], &bet_random_num, sizeof(int));
			
			balance += (random == bet_random_num ? 10 : -5);
			write(fd[0][1], &balance, sizeof(int));
		}
		keep_betting = 0;
		write(fd[0][1], &keep_betting, sizeof(int));
		close(fd[0][1]);
		close(fd[1][0]);
		wait(NULL);
	}
	
	return 0;
}
