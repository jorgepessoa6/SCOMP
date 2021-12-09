#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){
	pid_t p;
	if (fork() == 0) {
		printf("PID = %d\n", getpid());
		exit(0);
	}
	if ((p=fork()) == 0) {
		printf("PID = %d\n", getpid());
		exit(0);
	}
	printf("Parent PID = %d\n", getpid());
	printf("Waiting... (for PID=%d)\n",p);
	waitpid(p, NULL, 0);
	printf("Enter Loop...\n");
	while (1); /* Infinite loop */
	return 0;
} 
//A)

//Parent PID = 2636
//Waiting... (for PID=2638)
//PID = 2638
//Enter Loop...
//PID = 2637

//guest@porteus:/media/partilha/PL1/ex08$ ps
 //PID TTY          TIME CMD
 //2050 pts/1    00:00:00 bash
 //2635 pts/1    00:00:00 make
 //2636 pts/1    00:03:56 ex08
 //2637 pts/1    00:00:00 ex08 <defunct>
 //2682 pts/1    00:00:00 ps

//--Apenas o processo pai e o primeiro processo filho estao a ser executados. 
//--Uma vez que o processo pai espera que o segundo processo filho acabe garantimos que este
//--termina. Como nao sabemos como o escalanador reparte processos nao se pode deduzir nada em 
//--relacao a qual processo (pai ou primeiro filho) acaba primeiro

//B) o processo filho esta defunct, ou seja , ja nao esta em funcionamento
