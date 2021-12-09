ex17: ex17.o
	gcc -Wall -g ex17.o -o ex17.bin -lpthread -lrt

ex17.o: ex17.c
	gcc -Wall -g -c ex17.c

run: ex17
	./ex17.bin

clean:
	rm *.o *.bin
