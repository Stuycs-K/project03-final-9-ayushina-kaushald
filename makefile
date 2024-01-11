compile: client.o server.o queue.o words.o
	@gcc -o client client.o queue.o words.o
	@gcc -o server server.o queue.o words.o

client.o: client.c queue.h words.h
	@gcc -c client.c

server.o: server.c queue.h words.h
	@gcc -c server.c

queue.o: queue.c queue.h words.h
	@gcc -c queue.c

words.o: words.c queue.h words.h
	@gcc -c words.c

clean:
	rm *.o
