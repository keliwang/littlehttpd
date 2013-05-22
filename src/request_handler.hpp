#ifndef __REQUEST_HANDLER_HPP__
#define __REQUEST_HANDLER_HPP__

#include <boost/asio.hpp>
#include <string>
#include <sstream>
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
};

namespace http_get_helper {
	void handle_get_dir(std::string req_path, response& resp);
	void handle_get_file(std::string req_path, response& resp);
	void list_directory(std::string req_path, response& resp);
	bool url_decode(const std::string& in, std::string& out);
} // http_get_helper

namespace upload_helper {
	std::string get_boundary(const std::string& content_type);
	bool save_file(std::string& content, const std::string& boundary);

	std::string get_filename(const std::string& disposition);
	void skip_n_lines(std::istringstream& iss, int n);
	bool prepare_upload_dir(std::string path);

} // upload_helper

} // littlehttpd

#endif // __REQUEST_HANDLER_HPP__
