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
	void handle_upload_file(request& req, response& resp);
	bool is_upload_file(const std::string& content_type);
	void url_decode(const std::string& in, std::string& out);
};

namespace upload_helper {

} // upload_helper

} // littlehttpd

#endif // __REQUEST_HANDLER_HPP__
