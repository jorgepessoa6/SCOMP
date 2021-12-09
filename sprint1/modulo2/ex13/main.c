#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(void) {
	
	int  i, pieces_needed = 1000, pieces_a1 = 0, counter = 0, v_read, sequencia = 200;
	int pieces_m1 = 5, pieces_m2 = 5, pieces_m3 = 10, pieces_m4 = 100, done = 0;
	
	pid_t id_child;
	size_t buffer = sizeof(int);
	
	int fd_m1[2];
	int fd_m2[2];
	int fd_m3[2];
	int fd_m4[2];
	int fd_a[2];
	/* Create all pipes*/
	if(pipe(fd_m1) == -1){
		perror("\nErro a criar pipe\n");
		return 1;
	}
	
	if(pipe(fd_m2) == -1){
		perror("\nErro a criar pipe\n");
		return 1;
	}
	
	if(pipe(fd_m3) == -1){
		perror("\nErro a criar pipe\n");
		return 1;
	}
	
	if(pipe(fd_m4) == -1){
		perror("\nErro a criar pipe\n");
		return 1;
	}
	
	if(pipe(fd_a) == -1){
		perror("\nErro a criar pipe\n");
		return 1;
	}
	/* MACHINE 1 - CUTS 5 PIECES AT TIME */
	id_child = fork();
	
	if(id_child == 0){
		/* fecha descritor de escrita com pipe fd_a */
		/* fecha descritor de leitura com pipe fd_m1 */
		close(fd_a[1]);				
		close(fd_m1[0]);			
		
		while(pieces_needed > done){
			if(read(fd_a[0], &v_read, buffer) > 0) {
				counter += v_read;
				done += v_read;
				if(counter == 5) {
					printf("\nM1: CORTOU E MANDOU 5 PEÇAS PARA M2 || TOTAL: %d\n", done);
					write(fd_m1[1], &pieces_m1, buffer);
					counter = 0;
				}
			}
		}
		/* fecha descritor de leitura com pipe fd_a */
		/* fecha descritor de escrita com pipe fd_m1 */
		close(fd_a[0]);			
		close(fd_m1[1]);		
		exit(0);
	}
	/* MACHINE 2 - FOLDS 5 PIECES AT TIME */
	id_child = fork();
	
	if(id_child == 0){
		/* fecha descritor de escrita com pipe fd_m1 */
		/* fecha descritor de leitura com pipe fd_m2 */
		close(fd_m1[1]);		
		close(fd_m2[0]);		
		
		while(pieces_needed > done){
			if(read(fd_m1[0], &v_read, buffer) > 0) {
				done += v_read;
				printf("\nM2: DOBROU E MANDOU 5 PEÇAS PARA M3 || TOTAL: %d\n", done);
				write(fd_m2[1], &pieces_m2, buffer);
			}
		}
		/* fecha descritor de leitura com pipe fd_m1 */	
		/* fecha descritor de escrita com pipe fd_m2 */	
		close(fd_m1[0]);		
		close(fd_m2[1]);		
		exit(0);
	}
	/* MACHINE 3 - WELDS 10 PIECES AT TIME */
	id_child = fork();
	if(id_child == 0){
		/* fecha descritor de escrita com pipe fd_m2 */
		/* fecha descritor de leitura com pipe fd_m3 */
		close(fd_m2[1]);		
		close(fd_m3[0]);
		while(pieces_needed > done){
			if(read(fd_m2[0], &v_read, buffer) > 0) {
				counter += v_read;
				done += v_read;
				if(counter == 10) {
					printf("\nM3: SOLDOU E MANDOU 10 PEÇAS PARA M4 || TOTAL: %d\n", done);
					write(fd_m3[1], &pieces_m3, buffer);
					counter = 0;
				}
			}
		}
		/* fecha descritor de escrita com pipe fd_m3 */
		/* fecha descritor de leitura com pipe fd_m2 */
		close(fd_m2[0]);
		close(fd_m3[1]);
		exit(0);
	}
	
	/* MACHINE 4 - PACKS 100 PIECES AT TIME */
	id_child = fork();
	
	if(id_child == 0){
		/* fecha descritor de escrita com pipe fd_m3 */
		/* fecha descritor de leitura com pipe fd_m4 */
		close(fd_m3[1]);
		close(fd_m4[0]);
		
		while(pieces_needed > done){
			if(read(fd_m3[0], &v_read, buffer) > 0) {
				counter += v_read;
				done += v_read;
				if(counter == 100) {
					printf("\nM4: EMPACOTOU E MANDOU 100 PEÇAS PARA A1 || TOTAL: %d\n", done);
					write(fd_m4[1], &pieces_m4, buffer);
					counter = 0;
				}
			}
		}
		/* fecha descritor de escrita com pipe fd_m4 */
		/* fecha descritor de leitura com pipe fd_m3 */
		close(fd_m3[0]);
		close(fd_m4[1]);
		exit(0);
	}

	/* fecha descritor de leitura com pipe fd_a */
	close(fd_a[0]);
	for(i = 0; i < sequencia; i++) {
		write(fd_a[1], &pieces_m1, buffer);
	}
	/* fecha descritor de escrita com pipe fd_a */
	close(fd_a[1]);
	
	/* fecha descritor de escrita com pipe fd_m4 */
	close(fd_m4[1]);
	while(pieces_a1 < pieces_needed) {
		read(fd_m4[0], &counter, buffer);
		pieces_a1+=counter;
	}
	 
	/* fecha descritor de leitura com pipe fd_ma */
	close(fd_m4[0]);
		
	printf("\nArmazenamento A1: Expectado: %d ||  Obtido: %d \n\n", pieces_needed ,pieces_a1);
	return 0;
}
