CC = gcc
all:server client
OServer = server.o SrandPort.o
OClient = client.o SrandPort.o
server:${OServer}
	$(CC) -o $@ $^ -lpthread -D DEBUG
server.o:myServer.c
	$(CC) -c -o $@ $< -D DEBUG
SrandPort.o:SrandPort.c
	$(CC) -c -o $@ $< -D DEBUG
client:${OClient}
	$(CC) -o $@ $^ -lpthread -D DEBUG
client.o:myClient.c
	$(CC) -c  -o $@ $< -D DEBUG

.PHONY:clean
clean:
	-rm -rf *.o server client 

