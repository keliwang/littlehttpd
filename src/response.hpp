#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

#include "header.hpp"
#include <vector>
#include <string>
#include <boost/asio.hpp>

namespace littlehttpd {

struct response {
	// The status of the response.
	enum status_type {
		ok = 200,
		created = 201,
		accepted = 202,
		no_content = 204,
		multiple_choices = 300,
		moved_permanently = 301,
		moved_temporarily = 302,
		not_modified = 304,
		bad_request = 400,
		unauthorized = 401,
		forbidden = 403,
		not_found = 404,
		internal_server_error = 500,
		not_implemented = 501,
		bad_gateway = 502,
		service_unavailable = 503
	} status;

	// The headers to be included in the response.
	headers_t headers;

	// The content to be sent in the response.
	std::string content;

	// Convert the response into a vector of buffers. The buffers do not own the
	// underlying memory blocks, therefore the response object must remain valid and
	// not be changed until the write operation has completed.
	std::vector<boost::asio::const_buffer> to_buffers();

	// Get the default response.
	static response default_resp(status_type status);
};

} /* littlehttpd  */ 

#endif // __RESPONSE_HPP__
