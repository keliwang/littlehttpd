#include <cctype>
#include <boost/bind.hpp>
#include "server.hpp"

using namespace std;

namespace littlehttpd {

// 构造函数，初始化服务器环境
server::server(const string& port)
	: io_service_(),
	acceptor_(io_service_),
	new_connection_(),
	connection_manager_(),
	request_handler_()
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),
											atoi(port.c_str()));
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();

	start_accept();
}

// 启动服务器
void server::run()
{
	io_service_.run();
}

// Start accept connections 
void server::start_accept()
{
	new_connection_.reset(new connection(io_service_,
				connection_manager_, request_handler_));
	acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&server::handle_accept, this,
				boost::asio::placeholders::error));
}

void server::handle_accept(const boost::system::error_code& err)
{
	if (!acceptor_.is_open()) {
		return;
	}
	
	if (!err) {
		connection_manager_.start(new_connection_);
	}

	start_accept();
}

}
