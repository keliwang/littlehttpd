CC = g++
CPPFLAGS = -g -Wall --std=c++11 -lboost_system -lboost_regex -I/usr/include/boost

server: old_server.cc
	$(CC) $(CPPFLAGS) old_server.cc -o site/server

clean:
	rm -f server
