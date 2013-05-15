#include <ios>
#include <fstream>
#include <iostream>
#include <string>
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

	if (req.method == "GET") {
		handle_get(req, resp);
	} else if (req.method == "POST") {
		handle_post(req, resp);
	} else {
		resp = response::default_resp(response::bad_request);
	}

}

void request_handler::handle_get(request& req, response& resp)
{
	string request_path;
	url_decode(req.uri, request_path);

	ifstream is(request_path, ios::in | ios::binary);
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

void request_handler::handle_post(request& req, response& resp)
{
	if (is_upload_file(req.headers["Content-Type"])) {
		handle_upload_file(req, resp);
	} else {
		cout << req.content;
		cout.flush();
		resp = response::default_resp(response::accepted);
	}
}

void request_handler::handle_upload_file(request& req, response& resp)
{
	cout << req.content;
	cout.flush();
	resp = response::default_resp(response::created);
}

bool request_handler::is_upload_file(const string& content_type)
{
	if (content_type.find("multipart/form-data") == string::npos) {
		return false;
	}
	return true;
}
void request_handler::url_decode(const string& in, string& out)
{
	out.clear();

	if (in == "/") {
		out.append("index.html");
	} else {
		out.append(in.substr(1));
	}
}

} /* littlehttpd  */ 
