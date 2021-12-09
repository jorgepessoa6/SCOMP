#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
int main(){
	int fd[2];
	int fd2[2];
	pid_t p;
	if(pipe(fd) == -1){
		perror("Pipe failed");
	}
	if(pipe(fd2) == -1){
		perror("Pipe failed");
	}
	char str1[20];
	p= fork();
	if (p==0){
		scanf("%s",&str1);
		//write
		close(fd[0]);
		write(fd[1], &str1, sizeof(str1));
		close(fd[1]);
		//read
		close(fd2[1]);
		read(fd2[0], &str1,sizeof(str1));
		close(fd2[0]);
		printf("%s\n", str1);	
	}else{
		//read
		close(fd[1]);
		read(fd[0], &str1,sizeof(str1));
		int i;
		for (i = 0; str1[i]!='\0'; i++) {
			if(str1[i] >= 'a' && str1[i] <= 'z') {
				str1[i] = str1[i] - 32;
				continue;
			}
			 if(str1[i] >= 'A' && str1[i] <= 'Z') {
				str1[i] = str1[i] + 32;
			}
		}
		close(fd[0]);
		//write
		close(fd2[0]);
		write(fd2[1], &str1, sizeof(str1));
		close(fd2[1]);
	}
	return 0;
}
		
		
		
	
