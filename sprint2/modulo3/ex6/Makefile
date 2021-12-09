ex06: main.o
	gcc -Wall -g main.o -o ex06 -lrt
	
main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt
	
run: ex06
	./ex06
	
clean:
	rm main.o ex06