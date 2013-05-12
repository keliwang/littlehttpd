#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "request.hpp"
#include "response.hpp"
#include "request_handler.hpp"

namespace littlehttpd {

class connection_manager;

class connection
	: public boost::enable_shared_from_this<connection> {
public:
	explicit connection(boost::asio::io_service& io_service,
			connection_manager& manager,
			request_handler& handler);

	// Return the socket used by this connection
	boost::asio::ip::tcp::socket& socket();

	// Start handle connection
	void start();

	// Stop this connection
	void stop();

private:
	// socket used by a connection object
	boost::asio::ip::tcp::socket socket_;

	// request information
	request request_;

	// response information
	response response_;

	// connection manager
	connection_manager& connection_manager_;

	// request handler
	request_handler& request_handler_;
};

typedef boost::shared_ptr<connection> connection_ptr;

} /* littlehttpd  */ 

#endif // __CONNECTION_HPP__
