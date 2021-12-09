ex08: main.o
	gcc -Wall -g main.o -o ex08 -lrt
	
main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt
	
run: ex08
	./ex08
	
clean:
	rm main.o ex08
