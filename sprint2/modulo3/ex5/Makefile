ex5: ex5.o
	gcc -Wall -g ex5.o -o ex5.bin -lrt

ex5.o: ex5.c
	gcc -Wall -g -c ex5.c

run: ex5 
	./ex5.bin

clean:
	rm *.o *.bin
