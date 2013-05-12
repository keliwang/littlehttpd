#include <iostream>
#include "server.hpp"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		return 1;
	}

	littlehttpd::server srv(argv[1]);
	srv.run();

	return 0;
}
