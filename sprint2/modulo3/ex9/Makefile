ex9: ex9.o
	gcc -Wall -g ex9.o -o ex9.bin -lrt

ex9.o: ex9.c
	gcc -Wall -g -c ex9.c

run: ex9 
	./ex9.bin

clean:
	rm *.o *.bin
