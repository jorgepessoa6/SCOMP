#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
 fork(); fork(); fork();
 printf("SCOMP!\n");
 return 0;
} 
//A) São criados 7 processos .
//B) Ficheiro em anexo
//C) "SCOMP" é escrito 8 vezes. 
