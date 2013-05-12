#include <ctime>
#include <cctype>
#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

typedef map<string, string> Header;

void checkVersion(string ver)
{
	if (ver.substr(0, 5) != "HTTP/") {
		throw runtime_error("Wrong HTTP Version");
	}
}

pair<string, string> extractHeaderLine(const string& str, const string& splitter)
{
	auto splitter_pos = str.find(splitter);
	if (splitter_pos == string::npos) {
		throw runtime_error("Wrong HTTP Request Headers");
	}

	string first(str, 0, splitter_pos);
	auto splitter_len = splitter.length();
	auto second_start = splitter_pos + splitter_len;
	auto second_len = str.length() - second_start;
	string second(str, splitter_pos + splitter_len, second_len);

	pair<string, string> header_pair(first, second);

	return  header_pair;
}


Header extractHeaders(istream& in)
{
	Header headers;
	string header_line;

	while (getline(in, header_line)) {
		// Header已读取完成
		if (header_line == "\r")
			break;

		cout << "--------START------------" << endl;
		cout << header_line << endl;

		auto header_pair = extractHeaderLine(header_line, ": ");

		cout << header_pair.first << ": " << header_pair.second << endl;
		cout << "--------END-------------" << endl;
		headers[header_pair.first] = header_pair.second;
	}

	cout << "Header End" << endl;
	return headers;
}

string readData(istream& in)
{
	string data("");
	string line;

	while (getline(in, line)) {
		data += line + "\n";
	}

	return data;
}

void respNotFound(tcp::socket& client_socket)
{
	string msg, content("");
	ifstream not_found("404.html");
	while (getline(not_found, msg)) {
		content += msg + "\n";
	}

	boost::asio::write(client_socket, boost::asio::buffer("HTTP/1.1 404 Not Found\r\n"));
	boost::asio::write(client_socket, boost::asio::buffer("Content-Type: text/html; charset=utf-8\r\n"));
	boost::asio::write(client_socket, boost::asio::buffer("Connection: close\r\n"));
	boost::asio::write(client_socket, boost::asio::buffer("Content-Length: " + to_string(content.size()) + "\r\n"));
	boost::asio::write(client_socket, boost::asio::buffer("Server: LittleHttpd/0.1\r\n\r\n"));
	boost::asio::write(client_socket, boost::asio::buffer(content));
}
void handle_http_get(tcp::socket& client_socket, string path)
{
	auto resource_name = path.substr(1, path.length()-1);
	ifstream resource(resource_name);

	if (!resource) {
		respNotFound(client_socket);
	} else {
		boost::asio::write(client_socket, boost::asio::buffer("HTTP/1.1 200 OK\r\n"));
		boost::asio::write(client_socket, boost::asio::buffer("Content-Type: text/html; charset=utf-8\r\n"));
		boost::asio::write(client_socket, boost::asio::buffer("Connection: close\r\n"));
		boost::asio::write(client_socket, boost::asio::buffer("Server: LittleHttpd/0.1\r\n\r\n"));

		string msg;
		while (getline(resource, msg)) {
			boost::asio::write(client_socket, boost::asio::buffer(msg));
			boost::asio::write(client_socket, boost::asio::buffer("\n"));
		}
	}

}

string get_boundary(Header headers)
{
	auto type_line = headers["Content-Type"];
	auto start = type_line.find('=') + 1;

	return type_line.substr(start);
}

size_t get_data_size(Header headers)
{
	return atoi(headers["Content-Length"].c_str());
}

string get_filename(const string disposition)
{
	auto name_start = disposition.find("filename=\"") + string("filename=\"").length();
	auto name_len = disposition.length() - name_start - 2;

	return disposition.substr(name_start, name_len);
}

bool create_file(istream& resp, const string boundary)
{
	string boundary_start;
	if (getline(resp, boundary_start) &&
			boundary_start.find(boundary) == string::npos) {
		return false;
	}
	string disposition;
	if (!getline(resp, disposition)) {
		return false;
	}
	auto filename = get_filename(disposition);
	ofstream file;
	file.open(filename.c_str(), ios::out | ios::binary);

	string content_type;
	getline(resp, content_type);

	string content;
	while (getline(resp, content)) {
		if (content == "\r")
			continue;
		if (content.find(boundary) == string::npos) {
			file << content << "\n";
		}
	}
	file.close();

	cout << "File " << filename << " created." << endl;

	return true;
}

void handle_file_upload(tcp::socket& client_socket,
		string path, Header headers)
{
	auto boundary = get_boundary(headers);
	auto content_len = get_data_size(headers);

	boost::asio::streambuf response;
	istream resp_stream(&response);
	boost::system::error_code error;
	boost::asio::read(client_socket, response, boost::asio::transfer_exactly(content_len), error);

	if (create_file(resp_stream, boundary)) {
		boost::asio::write(client_socket, boost::asio::buffer("HTTP/1.1 201 CREATED\r\n"));
		boost::asio::write(client_socket, boost::asio::buffer("Content-Type: text/html; charset=utf-8\r\n"));
		boost::asio::write(client_socket, boost::asio::buffer("Connection: close\r\n"));
		boost::asio::write(client_socket, boost::asio::buffer("Server: LittleHttpd/0.1\r\n\r\n"));
		boost::asio::write(client_socket, boost::asio::buffer("<html> <head><title>Little Httpd 0.1</title></head> <body><h1>201 CREATED</h1></body> </html> \r\n"));
	} 

}

void handle_http_post(tcp::socket& client_socket,
		string path, Header headers)
{
	if (headers["Content-Type"].find("multipart/form-data;")
			!= string::npos) {
		handle_file_upload(client_socket, path, headers);
	}
}

void checkError(const boost::system::error_code& ec)
{
	if (ec != boost::asio::error::eof && ec) {
		throw boost::system::system_error(ec);
	}
}


int main(int argc, char *argv[])
{
	if (argc != 2) {
		cout << "Usage: server <port>" << endl;
		return 1;
	}

	try {
		boost::asio::io_service io_service;
		int port = atoi(argv[1]); // 监听端口号
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));

		// 循环等待客户端连接
		while (true) {
			tcp::socket client_socket(io_service);
			acceptor.accept(client_socket);
			boost::system::error_code error;

			boost::asio::streambuf response;

			auto len = boost::asio::read_until(client_socket, response, "\r\n", error);
			cout << "Len: " << len << endl;
			checkError(error);

			istream resp_stream(&response);
			string method, path, version;
			char lf, cr;
			resp_stream >> method >> path >> version >> lf >> cr;
			checkVersion(version);

			boost::asio::read_until(client_socket, response, "\r\n\r\n", error);
			checkError(error);

			auto headers = extractHeaders(resp_stream);

			if (method == "GET") {
				handle_http_get(client_socket, path);
			} else if (method == "POST") {
				handle_http_post(client_socket, path, headers);
			} else {
			}

			client_socket.close();
		}
	} catch (std::exception& e) {
		cout << e.what() << endl;
	}

	return 0;
}

