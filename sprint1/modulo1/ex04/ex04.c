#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(void) {
	int a=0, b, c, d;
	b = (int) fork();
	c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
	d = (int) getppid();
	a = a + 5;
	printf("\na=%d, b=%d, c=%d, d=%d\n",a,b,c,d);
	return 0;
}
//A) A variável A
//		No processo pai:
//		a = 0 + 5 = 5
//		b = PID do processo filho
//		c = PID do próprio processo
//		d = PID do processo pai deste

//		No processo filho:
//		a = 0 + 5 = 5
//		b = 0
//		c = PID do proprio processo
//		d = PID do processo pai

//B) Ficheiro em anexo

