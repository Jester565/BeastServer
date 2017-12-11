#include "HttpClient.h"
#include <boost/bind.hpp>

using namespace boost::beast;
HttpClient::HttpClient(socket_ptr socket, msg_handler msgHandler, disconnect_handler disHandler)
	:socket(socket), msgHandler(msgHandler), disHandler(disHandler)
{
	
}

void HttpClient::start()
{
	readRequest();
}

void HttpClient::send(boost::shared_ptr<http::response<http::string_body>> resp)
{
	respStore = resp;
	resp->prepare_payload();
	http::async_write(*socket, *resp, 
		boost::bind(&HttpClient::asyncSendHandler, 
			shared_from_this(), 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred, 
			resp->need_eof()));
}

HttpClient::~HttpClient()
{
}

void HttpClient::close()
{
	boost::system::error_code ec;
	socket->shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
	if (disHandler != nullptr) {
		disHandler(shared_from_this());
	}
}

void HttpClient::readRequest()
{
	req = CreateRequest();
	std::cout << buffer.size() << std::endl;
	//buffer.consume(buffer.size());
	http::async_read(*socket, buffer, *req,
		boost::bind(&HttpClient::asyncReceiveHandler, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void HttpClient::asyncReceiveHandler(const boost::system::error_code & error, unsigned int nBytes)
{
	if (error == http::error::end_of_stream) {
		close();
	}
		

	if (error) {
		std::cout << *req << std::endl;
		std::cout << req->body() << std::endl;
		std::cerr << "Receive Error: " << error.message() << std::endl;
		return;
	}
	/*
	std::cout << "RECEIVED: " << nBytes << std::endl;
	std::cout << req << std::endl;
	auto resp = CreateResponse();
	resp->result(200);
	resp->keep_alive(true);
	resp->set(http::field::server, "Beast");
	resp->set(http::field::content_type, "text/plain");
	resp->body() = (std::string)req->body();
	send(resp);
	*/
	if (msgHandler != nullptr) {
		msgHandler(shared_from_this(), req);
	}
	else {
		std::cout << "DEFAULT MESSAGE HANDLER: " << req << std::endl;
	}
	readRequest();
}

void HttpClient::asyncSendHandler(const boost::system::error_code & error, unsigned int nBytes, bool close)
{
	if (error) {
		std::cerr << "Send Error: " << error.message() << std::endl;
	}
	respStore = nullptr;
}
