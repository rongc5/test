CC=gcc
all:server client
server:server.o SrandPort.o
	$(CC) -o server server.o SrandPort.o -lpthread -D DEBUG
server.o:myServer.c
	$(CC) -c -o server.o myServer.c -D DEBUG
SrandPort.o:SrandPort.c
	$(CC) -c SrandPort.c -D DEBUG
client:client.o SrandPort.o
	$(CC) -o client client.o SrandPort.o -lpthread -D DEBUG
client.o:myClient.c
	$(CC) -c  -o client.o myClient.c -D DEBUG


clean:
	rm *.o server client

