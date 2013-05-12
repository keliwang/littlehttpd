#ifndef __CONNECTION_MANAGER_HPP__
#define __CONNECTION_MANAGER_HPP__

#include <set>
#include "connection.hpp"

namespace littlehttpd {

class connection_manager {
public:
	// Start a new connection
	void start(connection_ptr conn);

	// Stop a connection
	void stop(connection_ptr conn);

	// Stop all connections
	void stop_all();
private:
	std::set<connection_ptr> connections_;
};

} /* littlehttpd  */ 

#endif // __CONNECTION_MANAGER_HPP__
