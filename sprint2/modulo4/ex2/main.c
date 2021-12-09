#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h> 
#include <time.h>
#include <semaphore.h>

#define NUM_CHILD 8
#define NUM_PER_CHILD 2
#define NUM_SEM 8
#define NUM_FILES 2
#define SIZE_LINE 10

#define INPUT 0
#define OUTPUT 1

char *sem_names[NUM_SEM];
char *sem_names_files[NUM_FILES] = {"input.txt", "output.txt"};
int sem_values[NUM_SEM];

void preparar_ficheiros() {
	unlink(sem_names_files[INPUT]);
	unlink(sem_names_files[OUTPUT]);
	
	FILE * file = fopen(sem_names_files[INPUT],"w+");
	int num_nums = NUM_CHILD * NUM_PER_CHILD;
	int i;
	
	for(i = 0; i < num_nums; i++)
		fprintf(file, "%d\n", rand() % num_nums);
	
	fclose(file);
}

int main(){
	char numeros[NUM_PER_CHILD * NUM_CHILD][SIZE_LINE];
	char line[SIZE_LINE];
	sem_t *sems[NUM_SEM];
	int i;
	
	srand(time(NULL));
	
	for(i = 0; i < NUM_SEM; i++){
		sem_names[i] = malloc(100);
		sprintf(sem_names[i], "/sem_input_ex01_%d", i);
		
		if(i == 0)
			sem_values[i] = 2;
		else
			sem_values[i] = 0;
	}
	
	for(i = 0; i < NUM_SEM; i++)
		sems[i] = sem_open(sem_names[i], O_CREAT | O_EXCL, 0644, sem_values[i]);
		
	preparar_ficheiros();
	
	for(i = 0; i < NUM_CHILD; i++) {
		pid_t pid = fork();
		
		if(pid == -1) {
			printf("Erro a criar processo filho!\n");
			exit(-1);
		}
		
		if(pid == 0) {
			FILE *file;
			int start_line = i * NUM_PER_CHILD, end_line = (i + 1) * NUM_PER_CHILD;
			
			sem_wait(sems[i]);
			
				// Ler numeros do ficheiro input
				file = fopen(sem_names_files[INPUT], "r");
				
				int p = -1;	
				while (fgets(line, sizeof line, file) != NULL) {
					p++;					
					if(p < start_line)
						continue;
					
					if(p >= end_line)
						break;
					
					stpcpy(numeros[p], line);
				}
				fclose(file);
				
				sleep(1);
				
				printf("Filho %d leu.\n", i);
				
			
			sem_post(sems[(i + 1) % NUM_CHILD]);

			sem_wait(sems[i]);
			
				// Escrever numeros no ficheiro output
				file = fopen(sem_names_files[OUTPUT], "a");
				for(p = start_line; p < end_line; p++)
					fprintf(file, "%s", numeros[p]);
				fclose(file);
				
				sleep(1);
				
				printf("Filho %d escreveu.\n", i);
			
			sem_post(sems[(i + 1) % NUM_CHILD]);
			
			exit(0);
		}
	}
	
	for(i = 0; i < NUM_CHILD; i++)
		wait(NULL);
	
	for(i = 0; i < NUM_SEM; i++)
		sem_unlink(sem_names[i]);
	
	FILE *file;
	file = fopen(sem_names_files[OUTPUT], "r");
	while (fgets(line, sizeof line, file) != NULL)
		printf(line);
	fclose(file);
	
	return 0;
}