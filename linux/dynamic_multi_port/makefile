CC = gcc
all:server client
object = myClient.o myServer.o SrandPort.o
OServer = myServer.o SrandPort.o
OClient = myClient.o SrandPort.o

server:${OServer}
	$(CC)  -o $@ $^ -lpthread -D DEBUG
client:${OClient}
	$(CC)  -o $@ $^ -lpthread -D DEBUG 
$(filter %.o, $(object)): %.o: %.c 
	$(CC) -c -o  $@ $< -D DEBUG 

.PHONY:clean
clean:
	-rm -rf *.o server client 

