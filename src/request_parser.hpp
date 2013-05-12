#ifndef __REQUEST_PARSER_HPP__
#define __REQUEST_PARSER_HPP__

#include <boost/asio.hpp>
#include <string>
#include <sstream>
#include "request.hpp"

namespace littlehttpd {

class request_parser {
public:
	explicit request_parser(boost::asio::ip::tcp::socket& socket,
							request& req);

	void parser();

private:
	// Parser request line to get request method,
	// uri and http version
	void parser_request_line();

	// Parser all request headers into request_.headers
	void parser_headers();

	// Parser all other data if needed
	void parser_content();

	// Extract all headers into request
	void extract_headers(std::istringstream& header_lines_stream);

	// Extract one header line and return it
	header_t extract_one_header(const std::string& header_line);

	// Read socket until delim, and return a string
	std::string read_until(std::string delim);

	// socket connect with client
	boost::asio::ip::tcp::socket& socket_;

	// Buffer
	boost::asio::streambuf buffer_;

	request& request_;
};

} /* littlehttpd  */ 

#endif // __REQUEST_PARSER_HPP__
