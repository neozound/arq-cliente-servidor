CC	= g++ -std=c++11 -Wall -g
LDFLAGS = -lpthread -lzmqpp -lzmq

all: server client broker

server: server.cc
	$(CC) -o server server.cc $(LDFLAGS)

client: client.cc
	$(CC) -o client client.cc $(LDFLAGS)

broker: broker.cc
	$(CC) -o broker broker.cc $(LDFLAGS)

clean:
	rm -f client server

