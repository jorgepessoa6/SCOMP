ex12: main.o
	gcc -Wall -g main.o -o ex12 -lrt
	
main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt
	
run: ex12
	./ex12
	
clean:
	rm main.o ex12
