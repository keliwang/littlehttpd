#include "request_parser.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace littlehttpd {

request_parser::request_parser(
		boost::asio::ip::tcp::socket& socket,
		request& req)
	: socket_(socket),
	buffer_(),
	request_(req)
{
}


void request_parser::parser()
{
	parser_request_line();
	parser_headers();
	parser_content();
}

void request_parser::parser_request_line()
{
	auto request_line = read_until("\r\n");
	istringstream iss(request_line);

	iss >> request_.method;
	iss >> request_.uri;
	iss >> request_.http_version;
}

void request_parser::parser_headers()
{
	auto header_lines = read_until("\r\n\r\n");
	istringstream iss(header_lines);

	extract_headers(iss);
}

void request_parser::extract_headers(istringstream& header_lines_stream)
{
	string header_line;
	
	while (getline(header_lines_stream, header_line)) {
		if (header_line == "\r")
			break;

		auto header_pair = extract_one_header(header_line);
		request_.headers.insert(header_pair);
	}
}

header_t request_parser::extract_one_header(const string& header_line)
{
	string splitter(": ");

	auto splitter_pos = header_line.find(splitter);
	if (splitter_pos == string::npos) {
		throw runtime_error("Wrong HTTP Request Headers");
	}

	string first(header_line, 0, splitter_pos);

	auto splitter_len = splitter.length();
	auto second_start = splitter_pos + splitter_len;
	auto second_len = header_line.length() - second_start;
	string second(header_line, splitter_pos + splitter_len, second_len);

	header_t header_pair(first, second);

	return  header_pair;
}

string request_parser::read_until(string delim)
{
	auto length = boost::asio::read_until(socket_, buffer_, delim);
	//boost::asio::streambuf::const_buffers_type data = buffer_.data();
	auto data = buffer_.data();
	buffer_.consume(length);

	return string(
			boost::asio::buffers_begin(data),
			boost::asio::buffers_begin(data) + length);

}

void request_parser::parser_content()
{
	if (request_.headers.find("Content-Length") == request_.headers.end()) {
		return;
	}

	size_t content_length = stoul(request_.headers["Content-Length"]);

	boost::system::error_code err;
	boost::asio::read(socket_, buffer_,
			boost::asio::transfer_exactly(content_length - buffer_.size()), err);
	if (err == boost::asio::error::eof) {
		throw boost::system::system_error(err);
	}
	auto data = buffer_.data();
	buffer_.consume(content_length);

	request_.content = string(
				boost::asio::buffers_begin(data),
				boost::asio::buffers_begin(data) + content_length);
}

} /* littlehttpd  */ 
