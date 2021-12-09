#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
	int fd[2];
	struct Valores{
		int x;
		char str1[22];
	} v;
	pid_t p;
	if(pipe(fd) == -1){
		perror("Pipe failed");
	}
	p= fork();
	if (p>0){
		close(fd[0]);
		scanf("%d",&v.x);
		scanf("%s",&v.str1);
		write(fd[1], &v, sizeof(v));
		close(fd[1]);
	}else{
		close(fd[1]);
		read(fd[0],&v,sizeof(v));
		printf("%d\n", v.x);
		printf("%s\n", v.str1);
		close(fd[0]);
	}
	return 0;
}
	
