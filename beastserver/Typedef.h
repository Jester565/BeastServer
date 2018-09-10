#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <functional>

class HttpClient;

typedef boost::shared_ptr <boost::beast::http::request<boost::beast::http::string_body>> req_ptr;
typedef boost::shared_ptr <boost::beast::http::response<boost::beast::http::string_body>> resp_ptr;

static req_ptr CreateRequest() {
	return req_ptr(new boost::beast::http::request<boost::beast::http::string_body>());
}

static resp_ptr CreateResponse() {
	return resp_ptr(new boost::beast::http::response<boost::beast::http::string_body>());
}

typedef boost::shared_ptr<HttpClient> client_ptr;
typedef boost::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> ssl_socket;

typedef std::function<void(ssl_socket)> raw_connect_handler;
typedef std::function<void(client_ptr)> connect_handler;
typedef std::function<void(client_ptr, req_ptr)> msg_handler;
typedef std::function<void(client_ptr)> disconnect_handler;

typedef std::function<void(client_ptr, req_ptr)> evt_handler;