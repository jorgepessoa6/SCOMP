ex1: ex1.o
	gcc -Wall -g ex1.o -o ex1.bin -lpthread -lrt

ex1.o: ex1.c
	gcc -Wall -g -c ex1.c

run: ex1
	./ex1.bin

clean:
	rm *.o *.bin
