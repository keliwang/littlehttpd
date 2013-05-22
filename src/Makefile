CC=g++
CXXFLAGS=-g -Wall -std=c++11 -lboost_system -lboost_filesystem -I/usr/include/boost

main: server.o connection.o request_handler.o connection_manager.o request_parser.o response.o main.o
	$(CC) $(CXXFLAGS) server.o connection.o request_handler.o connection_manager.o request_parser.o response.o main.o -o main

server.o: server.cc
	$(CC) $(CXXFLAGS) -c server.cc -o server.o

connection.o: connection.cc
	$(CC) $(CXXFLAGS) -c connection.cc -o connection.o

request_handler.o: request_handler.cc
	$(CC) $(CXXFLAGS) -c request_handler.cc -o request_handler.o

connection_manager.o: connection_manager.cc
	$(CC) $(CXXFLAGS) -c connection_manager.cc -o connection_manager.o

request_parser.o: request_parser.cc
	$(CC) $(CXXFLAGS) -c request_parser.cc -o request_parser.o

response.o: response.cc
	$(CC) $(CXXFLAGS) -c response.cc -o response.o

main.o: main.cc
	$(CC) $(CXXFLAGS) -c main.cc -o main.o

clean: server.o connection.o request_handler.o connection_manager.o request_parser.o response.o main.o
	rm -f server.o connection.o request_handler.o connection_manager.o request_parser.o response.o main.o
