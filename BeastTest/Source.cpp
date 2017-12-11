#include <iostream>
#include <boost/beast.hpp>
#include "Server.h"
#include <boost/asio/io_service.hpp>

int main() {
	Server* server = new Server(5650);
	server->run();
	server->getIOService()->run();
}
