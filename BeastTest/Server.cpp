#include "Server.h"
#include "ServicePool.h"
#include "TCPAcceptor.h"

Server::Server()
{
	servicePool = boost::make_shared<ServicePool>(4);
	tcpAcceptor = boost::make_shared<TCPAcceptor>(servicePool);
}

void Server::run(uint16_t port)
{
	tcpAcceptor->detatch(port);
	servicePool->run();
}
