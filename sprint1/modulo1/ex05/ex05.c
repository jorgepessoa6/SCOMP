#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
	pid_t p = fork();
	pid_t p2;
	if(p ==0){
		sleep(1);
		printf("1");
	} else{
		p2 = fork();
		if(p2 ==0){
			sleep(2);
			printf("2");
		}
		wait();
	}
	return 0;
}
