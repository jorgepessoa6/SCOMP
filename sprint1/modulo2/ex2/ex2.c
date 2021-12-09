#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
	int fd[2];
	int x;
	char str1[20];
	pid_t p;
	if(pipe(fd) == -1){
		perror("Pipe failed");
	}
	p= fork();
	if (p>0){
		close(fd[0]);
		scanf("%d",&x);
		scanf("%s",str1);
		write(fd[1], &x, sizeof(int));
		write(fd[1], &str1, sizeof(str1));
		close(fd[1]);
	}else{
		close(fd[1]);
		read(fd[0],&x,sizeof(int));
		printf("%d\n", x);
		read(fd[0],&str1,sizeof(str1));
		printf("%s\n", str1);
		close(fd[0]);
	}
	return 0;
}
	
