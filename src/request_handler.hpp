#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include <boost/asio.hpp>
#include <string>
#include "request.hpp"
#include "response.hpp"

namespace littlehttpd {

class request_handler {
public:
	void handle_request(request& req, response& resp);
private:
	void handle_get(request& req, response& resp);
	void handle_post(request& req, response& resp);
	void url_decode(const std::string& in, std::string& out);
};

} /* littlehttpd  */ 

#endif // __REQUEST_HANDLER_HPP__
