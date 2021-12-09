#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef struct{
		int number1;
		int number2;
} numbers;
	
int main(void){
	int fd, data_size = sizeof(numbers),r;
	numbers *num;
	fd = shm_open("/ex5", O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate (fd, data_size);
	num = (numbers*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	
	num->number1=8000;
	num->number2=200;
	
	int i;
	pid_t pid=fork();
	if (pid>0){
		for (i = 0; i < 1000000; i++){
			num->number1++;
			num->number2--;
		}
		wait(NULL);
		printf("Total: %d\n",num->number1);
		printf("Total: %d\n",num->number2);
		
	}else{
		for (i = 0; i < 1000000; i++){
			num->number1--;
			num->number2++;
		}
	}
	r = munmap(num, data_size); /* disconnects */
	if (r < 0) exit(1); /* Check error */
	r = shm_unlink("/ex5"); /* removes */
	if (r < 0) exit(1); /* Check error */
	
	return 0;
}
/** Sim, uma vez que apenas verificamos os resultados no fim. Garantimos assim que todas as operacoes estao feitas, sendo que a ordem 
 *  nao interessa pois apenas verificamos os resultados apos ambos os processos acabarem com todas as suas operacoes
 */

