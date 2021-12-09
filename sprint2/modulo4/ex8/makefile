ex8:  main.o 
	gcc -Wall -g  main.o -o ex8 -lrt -lpthread
	
main.o: main.c
	gcc -Wall -g -c main.c -o main.o -lrt -lpthread
		
run: ex8
	./ex8
		
clean:
	rm  main.o ex8
