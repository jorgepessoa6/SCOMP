ex16:  main.o 
	gcc -Wall -g  main.o -o ex16 -lrt -lpthread
	
main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt -lpthread
		
run: ex16
	./ex16
		
clean:
	rm  main.o ex16
