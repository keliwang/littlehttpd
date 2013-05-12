#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <string>
#include <boost/asio.hpp>
#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"

namespace littlehttpd {

class server {
public:
	explicit server(const std::string& port);

	// Start running our http server
	void run();

	// Start accept requests
	void start_accept();

	// Handle an accept
	void handle_accept(const boost::system::error_code& err);

private:
	// Main io service
	boost::asio::io_service io_service_;
	
	// Used to accept request from client
	boost::asio::ip::tcp::acceptor acceptor_;

	// Point to a connection
	connection_ptr new_connection_;

	// Used to manage all connections
	connection_manager connection_manager_;

	// Used to handle request
	request_handler request_handler_;
};

} // littlehttpd

#endif // __SERVER_HPP__
