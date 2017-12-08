#pragma once
#include <stdint.h>
#include <thread>
#include <boost/asio/ip/tcp.hpp>
#include <boost/shared_ptr.hpp>

class ServicePool;
class TCPAcceptor;

class Server {
public:
	Server();

	void run(uint16_t port);

private:
	boost::shared_ptr<ServicePool> servicePool;
	boost::shared_ptr<TCPAcceptor> tcpAcceptor;
};