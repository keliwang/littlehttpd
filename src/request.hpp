#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include <string>
#include "header.hpp"

namespace littlehttpd {

struct request {
	std::string method;
	std::string uri;
	std::string http_version;
	headers_t headers;

	// Other data sent by client, such as
	// data posted by client
	std::string content;
};

} /* littlehttpd  */ 

#endif // __REQUEST_HPP__
