#pragma once
#include "Typedef.h"
#include <stdint.h>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include <boost/shared_ptr.hpp>

class TCPAcceptor;
class ClientManager;
class EventManager;

class Server {
public:
	Server(uint16_t port);

	void run();

	boost::asio::io_service* getIOService() {
		return ioService;
	}

private:
	boost::shared_ptr<TCPAcceptor> tcpAcceptor;

	ClientManager* clientManager;
	EventManager* evtManager;

	void rawConnectHandler(socket_ptr);
	void msgHandler(client_ptr, req_ptr);
	void disconnectHandler(client_ptr);

	boost::asio::io_service* ioService;
};