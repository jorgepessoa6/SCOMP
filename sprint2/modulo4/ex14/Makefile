program: reader writer

reader:  reader.o 
		gcc -Wall -g  reader.o -o reader -lrt -lpthread
	
reader.o: reader.c
		gcc -Wall -g -c reader.c -o reader.o -lrt -lpthread

writer:  writer.o 
		gcc -Wall -g  writer.o -o writer -lrt -lpthread
	
writer.o: writer.c
		gcc -Wall -g -c writer.c -o writer.o -lrt -lpthread

run: writer reader
	./writer
	./reader
		
clean:
		rm  writer.o writer reader.o reader 
