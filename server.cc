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
void handle_http_get(tcp::socket& client_socket,
		string path, Header headers)
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
			boost::asio::read_until(client_socket, response, "\r\n", error);
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
				handle_http_get(client_socket, path, headers);
			} else if (method == "POST") {
				auto postData = readData(resp_stream);
				// TODO: 解析接收到的POST数据
				size_t content_len = atoi(headers["Content-Length"].c_str());
				boost::asio::read(client_socket, response, boost::asio::transfer_exactly(content_len), error);
				checkError(error);
			} else {
			}

			client_socket.close();
		}
	} catch (std::exception& e) {
		cout << e.what() << endl;
	}

	return 0;
}
 
