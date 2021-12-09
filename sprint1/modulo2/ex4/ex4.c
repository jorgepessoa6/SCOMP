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
	char buff[20];
	p= fork();
	if (p>0){
		FILE *fp=fopen("/media/partilha/scomp_2df_g30/sprint2/ex4/xD.txt", "r");
		fscanf(fp, "%s", buff);
		close(fd[0]);
		write(fd[1], &buff, sizeof(fp));
		close(fd[1]);
		wait();
		fclose(fp);
	}else{
		close(fd[1]);
		read(fd[0], &buff, sizeof(buff));
		close(fd[0]);
		printf("%s\n", buff);
	}
	return 0;
}
		
	
