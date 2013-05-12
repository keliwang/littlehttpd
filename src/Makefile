CC=g++
CXXFLAGS=-g -Wall

main: server.o connection_manager.o response.o main.o
	$(CC) $(CXXFLAGS) server.o connection_manager.o response.o main.o -o main

server.o: server.cc
	$(CC) $(CXXFLAGS) -c server.cc -o server.o

connection_manager.o: connection_manager.cc
	$(CC) $(CXXFLAGS) -c connection_manager.cc -o connection_manager.o

response.o: response.cc
	$(CC) $(CXXFLAGS) -c response.cc -o response.o

main.o: main.cc
	$(CC) $(CXXFLAGS) -c main.cc -o main.o

clean: server.o connection_manager.o response.o main.o
	rm -f server.o connection_manager.o response.o main.o
