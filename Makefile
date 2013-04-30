CC = g++
CPPFLAGS = -g -Wall --std=c++11 -lboost_system -I/usr/include/boost

server: server.cc
	$(CC) $(CPPFLAGS) server.cc -o site/server

clean:
	rm -f server
