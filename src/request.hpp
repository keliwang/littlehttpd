#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include <string>
#include <vector>
#include "header.hpp"

namespace littlehttpd {

struct request {
	std::string method;
	std::string uri;
	std::string http_version;
	std::vector<header> headers;
};

} /* littlehttpd  */ 

#endif // __REQUEST_HPP__
