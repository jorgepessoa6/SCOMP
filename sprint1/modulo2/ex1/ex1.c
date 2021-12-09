#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
	int fd[2];
	pid_t p;
	if(pipe(fd) == -1){
		perror("Pipe failed");
	}
	p= fork();
	if (p>0){
		close(fd[0]);
		printf("pid verdadeiro %d\n",getpid());
		int y = getpid();
		write(fd[1], &y,sizeof(int));
		close(fd[1]);
	}else{		
		close(fd[1]);
		int x;
		read(fd[0],&x,sizeof(int));
		printf("O pid do pai: %d\n", x);
		close(fd[0]);
	}
	return 0;
}
	
	
