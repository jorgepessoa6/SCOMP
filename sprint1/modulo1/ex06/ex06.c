#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void main(){
	int i;
	pid_t p[4];
	for (i = 0; i < 4; i++) {
		p[i]=fork();
		if (p[i] == 0) {
			break;
		} else{
			if (p[i]%2 ==0){
				wait();
			}
		}
	}
	printf("This is the end. %d %d\n",i, getpid());

} 

	//A) 16 processos são criados. A process tree encontra-se em anexo.
	//B) break se for filho



