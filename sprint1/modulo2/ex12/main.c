#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_LEITORES 5
#define NUM_PRODUTOS 20

typedef struct{
	char* nome;
	int codigo;
	int preco;
	int leitor;
}	Produto;

int main(void) {
	int shared_pipe[2];
	int readers_pipes[NUM_LEITORES][2];
	
	int i;
	Produto produto;
	
	if(pipe(shared_pipe) == -1) {
		printf("Erro a criar pipe partilhado!");
		return 0;
	}
	
	for(i = 0; i < NUM_LEITORES; i++) {
		if(pipe(readers_pipes[i]) == -1) {
			printf("Erro a criar pipe para filho!");
			return 0;
		}
	}
	
	for(i = 0; i < NUM_LEITORES; i++) {
		if(fork() == 0) {
			close(shared_pipe[0]);
			close(readers_pipes[i][1]);
			
			scanf("%d", &produto.codigo);
			produto.leitor = i;
			
			write(shared_pipe[1], &produto, sizeof(produto));
			close(shared_pipe[1]);
			
			read(readers_pipes[i][0], &produto, sizeof(produto));
			close(readers_pipes[i][0]);
			
			printf("Codigo: %d | Nome: %s | Preco: %d\n", produto.codigo, produto.nome, produto.preco);
			
			exit(0);
		}
	}
	
	close(shared_pipe[1]);
	
	for(i = 0; i < NUM_LEITORES; i++)
		close(readers_pipes[i][0]);
	
	Produto produtos[NUM_PRODUTOS];
	for(i = 0; i < NUM_PRODUTOS; i++) {
		produtos[i].codigo = i;
		produtos[i].preco = (i + 1) * 13;
		produtos[i].nome = "XPTO";
	}
	
	while(read(shared_pipe[0], &produto, sizeof(produto)) > 0) {
		for(i = 0; i < NUM_PRODUTOS; i++) {
			if(produtos[i].codigo == produto.codigo) {
				produtos[i].leitor = produto.leitor;
				write(readers_pipes[produto.leitor][1], &produtos[i], sizeof(produtos[i]));
				close(readers_pipes[produto.leitor][1]);
			}
		}
	}
	
	close(shared_pipe[0]);
	
	return 0;
}
