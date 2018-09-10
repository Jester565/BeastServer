#pragma once
#include "Typedef.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <shared_mutex>
#include <queue>

using namespace boost::beast;

class HttpClient : public boost::enable_shared_from_this<HttpClient>
{
public:
	HttpClient(ssl_socket socket, msg_handler msgHandler = nullptr, disconnect_handler disHandler = nullptr);

	void start();

	void send(boost::shared_ptr<boost::beast::http::response<boost::beast::http::string_body>> resp);

	~HttpClient();

private:
	void close();

	void readRequest();
	bool asyncVerifyHandler(bool preverified, boost::asio::ssl::verify_context& verifyCtx);
	void asyncHandshakeHandler(const boost::system::error_code& error);
	void asyncReceiveHandler(const boost::system::error_code& error, unsigned int nBytes);
	void asyncSendHandler(const boost::system::error_code& error, unsigned int nBytes, bool close);
	std::string subject;
	ssl_socket socket;

	boost::beast::flat_buffer buffer;
	req_ptr req;
	resp_ptr respStore;

	msg_handler msgHandler;
	disconnect_handler disHandler;
	//std::queue<boost::shared_ptr<boost::beast::http::response<http::string_body>>> respStore;
	//boost::shared_mutex queueMutex;
};

