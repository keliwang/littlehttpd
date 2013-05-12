#include "connection.hpp"

namespace littlehttpd {

connection::connection(boost::asio::io_service& io_service,
			connection_manager& manager,
			request_handler& handler)
	: socket_(io_service),
	request_parser_(socket_, request_),
	connection_manager_(manager),
	request_handler_(handler)
{
}

boost::asio::ip::tcp::socket& connection::socket()
{
	return socket_;
}

void connection::start()
{
	// Read request and parser it
	handle_read();

	// Send response
	handle_write();
}

void connection::stop()
{
	socket_.close();
}

void connection::handle_read()
{
	request_parser_.parser();
	request_handler_.handle_request(request_, response_);
}

void connection::handle_write()
{
	boost::asio::write(socket_, response_.to_buffers());
	stop();
}

} /* littlehttpd  */ 
