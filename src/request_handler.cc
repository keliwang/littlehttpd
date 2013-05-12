#include <string>
#include <ios>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include "request_handler.hpp"

using namespace std;

namespace littlehttpd {

void request_handler::handle_request(request& req, response& resp)
{
	if (req.uri.empty() || req.uri[0] != '/'
			|| req.uri.find("..") != string::npos) {
		resp = response::default_resp(response::bad_request);
		return;
	}

	ifstream is(req.uri.substr(1), ios::in | ios::binary);
	if (!is) {
		resp = response::default_resp(response::not_found);
		return;
	}

	// Fill out the response to be sent to the client.
	resp.status = response::ok;
	char buf[512];
	while (is.read(buf, sizeof(buf)).gcount() > 0) {
		resp.content.append(buf, is.gcount());
	}
	resp.headers["Content-Length"] = boost::lexical_cast<string>(resp.content.size());
}

} /* littlehttpd  */ 
