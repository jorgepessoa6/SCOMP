writer: writer.o
	gcc -Wall -g writer.o -o writer -lrt

writer.o: writer.c
	gcc -Wall -g -c writer.c

reader: reader.o
	gcc -Wall -g reader.o -o reader -lrt

reader.o: reader.c
	gcc -Wall -g -c reader.c

run: writer reader
	./writer
	./reader

run-writer: writer
	./writer

run-reader: reader
	./reader

clean:
	rm *.o writer
	rm *.o reader
