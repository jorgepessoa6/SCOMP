#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
int main(){
	int fd[2];
	pid_t p;
	if(pipe(fd) == -1){
		perror("Pipe failed");
	}
	int status;
	p= fork();
	if (p>0){
		char str1[20]= "hello world";
		char str2[20]= "Goodbye!";	
		close(fd[0]);
		write(fd[1], &str1, sizeof(str1));
		write(fd[1], &str2, sizeof(str2));
		wait(&status);
		printf("O valor de saida do filho foi de: %d\n", WEXITSTATUS(status));
		printf("O valor de pid do filho é de:%d\n", p);
		close(fd[1]);
		
	}else{
		printf("Valor verdadeiro: %d\n", getpid());
		close(fd[1]);
		char str1[20];
		char str2[20];
		read(fd[0], &str1, sizeof(str1));
		printf("%s\n", str1);
		read(fd[0], &str2, sizeof(str2));
		printf("%s\n", str2);
		close(fd[0]);
		exit(0);
	}
	return 0;
}
	
