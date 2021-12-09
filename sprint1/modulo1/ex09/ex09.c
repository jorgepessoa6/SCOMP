#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
	pid_t p[10];
	int i;
	int j;
	for (i = 0; i < 10; i++){
		p[i] = fork();
		if (p[i]==0){
			for (j = 0; j < 100; j++){
				printf("Numero: %d\n", 100*i+j);
			}
			exit(0);
		}
	}
	for (i = 0; i < 10; i++){
		wait();
	}	
	printf("Esperei como um bom pai \n");
	return 0;
}


//a) Não é possivel garantir que o resultado é sorted porque não sabemos como é que funciona o escalonador.
	
