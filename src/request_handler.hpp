#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include <boost/asio.hpp>
#include "request.hpp"
#include "response.hpp"

namespace littlehttpd {

class request_handler {
public:
	void handle_request(request& req, response& resp);
};

} /* littlehttpd  */ 

#endif // __REQUEST_HANDLER_HPP__
