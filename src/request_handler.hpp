#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include "request.hpp"
#include <boost/asio.hpp>

namespace littlehttpd {

class request_handler {
public:
	explicit request_handler();

	void handle();
private:
};

} /* littlehttpd  */ 

#endif // __REQUEST_HANDLER_HPP__
