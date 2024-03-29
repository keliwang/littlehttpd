#include "response.hpp"
#include <string>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace littlehttpd {

namespace status_strings {

	const string ok =
		"HTTP/1.0 200 OK\r\n";
	const string created =
		"HTTP/1.0 201 Created\r\n";
	const string accepted =
		"HTTP/1.0 202 Accepted\r\n";
	const string no_content =
		"HTTP/1.0 204 No Content\r\n";
	const string multiple_choices =
		"HTTP/1.0 300 Multiple Choices\r\n";
	const string moved_permanently =
		"HTTP/1.0 301 Moved Permanently\r\n";
	const string moved_temporarily =
		"HTTP/1.0 302 Moved Temporarily\r\n";
	const string not_modified =
		"HTTP/1.0 304 Not Modified\r\n";
	const string bad_request =
		"HTTP/1.0 400 Bad Request\r\n";
	const string unauthorized =
		"HTTP/1.0 401 Unauthorized\r\n";
	const string forbidden =
		"HTTP/1.0 403 Forbidden\r\n";
	const string not_found =
		"HTTP/1.0 404 Not Found\r\n";
	const string internal_server_error =
		"HTTP/1.0 500 Internal Server Error\r\n";
	const string not_implemented =
		"HTTP/1.0 501 Not Implemented\r\n";
	const string bad_gateway =
		"HTTP/1.0 502 Bad Gateway\r\n";
	const string service_unavailable =
		"HTTP/1.0 503 Service Unavailable\r\n";

	boost::asio::const_buffer to_buffer(response::status_type status)
	{
		switch (status) {
		case response::ok:
			return boost::asio::buffer(ok);
		case response::created:
			return boost::asio::buffer(created);
		case response::accepted:
			return boost::asio::buffer(accepted);
		case response::no_content:
			return boost::asio::buffer(no_content);
		case response::multiple_choices:
			return boost::asio::buffer(multiple_choices);
		case response::moved_permanently:
			return boost::asio::buffer(moved_permanently);
		case response::moved_temporarily:
			return boost::asio::buffer(moved_temporarily);
		case response::not_modified:
			return boost::asio::buffer(not_modified);
		case response::bad_request:
			return boost::asio::buffer(bad_request);
		case response::unauthorized:
			return boost::asio::buffer(unauthorized);
		case response::forbidden:
			return boost::asio::buffer(forbidden);
		case response::not_found:
			return boost::asio::buffer(not_found);
		case response::internal_server_error:
			return boost::asio::buffer(internal_server_error);
		case response::not_implemented:
			return boost::asio::buffer(not_implemented);
		case response::bad_gateway:
			return boost::asio::buffer(bad_gateway);
		case response::service_unavailable:
			return boost::asio::buffer(service_unavailable);
		default:
			return boost::asio::buffer(internal_server_error);
		}
	}

} // namespace status_strings

namespace misc_strings {

	const char name_value_separator[] = { ':', ' ' };
	const char crlf[] = { '\r', '\n' };

} // namespace misc_strings

void push_server_name(vector<boost::asio::const_buffer>& resp_buf)
{
	const string server("Server");
	const string server_version("Littlehttpd/1.0");

	resp_buf.push_back(boost::asio::buffer(server));
	resp_buf.push_back(boost::asio::buffer(misc_strings::name_value_separator));
	resp_buf.push_back(boost::asio::buffer(server_version));
	resp_buf.push_back(boost::asio::buffer(misc_strings::crlf));
}

vector<boost::asio::const_buffer> response::to_buffers()
{
	vector<boost::asio::const_buffer> buffers;
	buffers.push_back(status_strings::to_buffer(status));
	for (auto header:headers) {
		buffers.push_back(boost::asio::buffer(header.first));
		buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
		buffers.push_back(boost::asio::buffer(header.second));
		buffers.push_back(boost::asio::buffer(misc_strings::crlf));
	}
	push_server_name(buffers);
	buffers.push_back(boost::asio::buffer(misc_strings::crlf));
	buffers.push_back(boost::asio::buffer(content));
	return buffers;
}

namespace default_responses {

	const char ok[] = "";
	const char created[] =
		"<html>"
		"<head><title>Created</title></head>"
		"<body><h1>201 Created</h1></body>"
		"</html>";
	const char accepted[] =
		"<html>"
		"<head><title>Accepted</title></head>"
		"<body><h1>202 Accepted</h1></body>"
		"</html>";
	const char no_content[] =
		"<html>"
		"<head><title>No Content</title></head>"
		"<body><h1>204 Content</h1></body>"
		"</html>";
	const char multiple_choices[] =
		"<html>"
		"<head><title>Multiple Choices</title></head>"
		"<body><h1>300 Multiple Choices</h1></body>"
		"</html>";
	const char moved_permanently[] =
		"<html>"
		"<head><title>Moved Permanently</title></head>"
		"<body><h1>301 Moved Permanently</h1></body>"
		"</html>";
	const char moved_temporarily[] =
		"<html>"
		"<head><title>Moved Temporarily</title></head>"
		"<body><h1>302 Moved Temporarily</h1></body>"
		"</html>";
	const char not_modified[] =
		"<html>"
		"<head><title>Not Modified</title></head>"
		"<body><h1>304 Not Modified</h1></body>"
		"</html>";
	const char bad_request[] =
		"<html>"
		"<head><title>Bad Request</title></head>"
		"<body><h1>400 Bad Request</h1></body>"
		"</html>";
	const char unauthorized[] =
		"<html>"
		"<head><title>Unauthorized</title></head>"
		"<body><h1>401 Unauthorized</h1></body>"
		"</html>";
	const char forbidden[] =
		"<html>"
		"<head><title>Forbidden</title></head>"
		"<body><h1>403 Forbidden</h1></body>"
		"</html>";
	const char not_found[] =
		"<html>"
		"<head><title>Not Found</title></head>"
		"<body><h1>404 Not Found</h1></body>"
		"</html>";
	const char internal_server_error[] =
		"<html>"
		"<head><title>Internal Server Error</title></head>"
		"<body><h1>500 Internal Server Error</h1></body>"
		"</html>";
	const char not_implemented[] =
		"<html>"
		"<head><title>Not Implemented</title></head>"
		"<body><h1>501 Not Implemented</h1></body>"
		"</html>";
	const char bad_gateway[] =
		"<html>"
		"<head><title>Bad Gateway</title></head>"
		"<body><h1>502 Bad Gateway</h1></body>"
		"</html>";
	const char service_unavailable[] =
		"<html>"
		"<head><title>Service Unavailable</title></head>"
		"<body><h1>503 Service Unavailable</h1></body>"
		"</html>";

	string to_string(response::status_type status)
	{
		switch (status) {
			case response::ok:
				return ok;
			case response::created:
				return created;
			case response::accepted:
				return accepted;
			case response::no_content:
				return no_content;
			case response::multiple_choices:
				return multiple_choices;
			case response::moved_permanently:
				return moved_permanently;
			case response::moved_temporarily:
				return moved_temporarily;
			case response::not_modified:
				return not_modified;
			case response::bad_request:
				return bad_request;
			case response::unauthorized:
				return unauthorized;
			case response::forbidden:
				return forbidden;
			case response::not_found:
				return not_found;
			case response::internal_server_error:
				return internal_server_error;
			case response::not_implemented:
				return not_implemented;
			case response::bad_gateway:
				return bad_gateway;
			case response::service_unavailable:
				return service_unavailable;
			default:
				return internal_server_error;
		}
	}

} // namespace default_responses

response response::default_resp(response::status_type status)
{
	response resp;
	resp.status = status;
	resp.content = default_responses::to_string(status);
	resp.headers["Content-Length"] = boost::lexical_cast<string>(resp.content.size());
	resp.headers["Content-Type"] = "text/html";
	return resp;
}

} // namespace littlehttpd
