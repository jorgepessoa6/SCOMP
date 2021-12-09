ex02:  main.o 
	gcc -Wall -g  main.o -o ex02 -lrt -lpthread
	
main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt -lpthread
		
run: ex02
	./ex02
		
clean:
	rm  main.o ex02 input.txt output.txt
