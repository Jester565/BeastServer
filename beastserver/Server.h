#pragma once
#include "Typedef.h"
#include <stdint.h>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ssl/context.hpp>

class TCPAcceptor;
class ClientManager;
class EventManager;

class Server {
public:
	Server(uint16_t port, const std::string& chainFile, const std::string& keyFile, const std::string& verifyPath);

	void run();

	boost::asio::io_service* getIOService() {
		return ioService;
	}

private:
	boost::shared_ptr<boost::asio::ssl::context> sslContext;
	boost::shared_ptr<TCPAcceptor> tcpAcceptor;

	ClientManager* clientManager;
	EventManager* evtManager;

	void rawConnectHandler(ssl_socket);
	void msgHandler(client_ptr, req_ptr);
	void disconnectHandler(client_ptr);

	boost::asio::io_service* ioService;
};