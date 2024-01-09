compile: client.o server.o
	@gcc -o client client.o queue.o
	@gcc -o server server.o queue.o

client.o: client.c queue.h
	@gcc -c client.c

server.o: server.c queue.h
	@gcc -c server.c

queue.o: queue.c queue.h
	@gcc -c queue.c

clean:
	rm *.o
