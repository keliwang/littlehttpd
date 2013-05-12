#ifndef __HEADER_HPP__
#define __HEADER_HPP__

#include <map>
#include <string>

namespace littlehttpd {

// A new type to save http header
typedef std::pair<std::string, std::string> header_t;

typedef std::map<std::string, std::string> headers_t;

} // littlehttpd

#endif // __HEADER_HPP__
