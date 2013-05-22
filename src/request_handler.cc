#include <ios>
#include <fstream>
#include <iostream>
#include <string>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
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
	auto boundary = upload_helper::get_boundary(req.headers["Content-Type"]);
	auto succ = upload_helper::save_file(req.content, boundary);
	if (succ) {
		resp = response::default_resp(response::created);
	} else {
		resp = response::default_resp(response::internal_server_error);
	}
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

namespace upload_helper {

	string get_boundary(const string& content_type)
	{
		auto boundary_start_pos = content_type.find("=") + 1;

		return content_type.substr(boundary_start_pos);
	}

	bool save_file(string& content, const string& boundary)
	{
		istringstream content_stream(content);

		string start_boundary;
		if (!getline(content_stream, start_boundary)) {
			return false;
		}
		if (start_boundary != "--" + boundary) {
			return false;
		}

		string content_disposition;
		if (!getline(content_stream, content_disposition)) {
			return false;
		}
		auto filename = get_filename(content_disposition);

		// Skip 2 lines:
		//   1. A Content-Type line
		//   2. A blank line
		skip_n_lines(content_stream, 2);

		// end_boundary = "--" + boundary + "--\r\n"
		// So, end_boundary's length is boundary.size() + 6
		long end_boundary_len = boundary.size() + 6;

		const string upload_dir("upload");
		if (!prepare_upload_dir(upload_dir)) {
			throw runtime_error("Create upload directory fail");
		}

		const boost::filesystem::path upload_file_path = 
				boost::filesystem::path(upload_dir) / boost::filesystem::path(filename);
		ofstream output_file(upload_file_path.c_str(), ios::out | ios::binary);
		if (!output_file) {
			return false;
		}

		char buf[512];
		while (content_stream.read(buf, sizeof(buf)).gcount() > end_boundary_len) {
			output_file.write(buf, sizeof(buf));
		}
		output_file.close();

		return true;
	}

	string get_filename(const string& disposition)
	{
		auto filename_pos = disposition.find("filename");
		auto filename_start_pos = disposition.find("\"", filename_pos) + 1;
		auto filename_end_pos = disposition.find_last_of("\"");
		auto filename_len = filename_end_pos - filename_start_pos;

		return disposition.substr(filename_start_pos, filename_len);
	}

	void skip_n_lines(std::istringstream& iss, int n)
	{
		for (auto i = 0; i != n; i++) {
			string discard_str;
			getline(iss, discard_str);
		}
	}

	bool prepare_upload_dir(std::string path)
	{
		boost::filesystem::path upload_path(path);

		// If upload directory already exsits, return true
		if (boost::filesystem::exists(upload_path)) {
			return true;
		}

		// Create upload directory now
		try {
			boost::filesystem::create_directory(upload_path);
		} catch (exception& e) {
			return false;
		}

		return true;
	}

} // upload_helper

} // littlehttpd
