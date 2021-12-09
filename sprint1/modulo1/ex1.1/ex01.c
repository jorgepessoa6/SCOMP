#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
	int x = 1;
	pid_t p = fork(); /*pid_t: sys/types.h; fork(): unistd.h*/
	if (p == 0) {
		x = x+1;
		printf("1. x = %d\n", x);
	} else {
		x = x-1;
		printf("2. x = %d\n", x);
	}
	printf("3. %d; x = %d\n", p, x);
	return 0;
} 
//A) O output deste código será estas quatro linhas por ordem aleatória:
//		1. x = 2
//  	2. x = 0
//		3. 0; x = 2
//		3. 1234; x = 0

//B) Não, pois o processo filho pode correr o printf() da linha 11 antes do pai correr a linha 9.	
