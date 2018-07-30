#include <iostream>
#include <boost/beast.hpp>
#include "Server.h"
#include <boost/asio/io_service.hpp>

int main() {
	Server* server = new Server(5650, "C:/Users/ajcra/Desktop/north/ssl/localhost.crt", "C:/Users/ajcra/Desktop/north/ssl/localhost.key", "C:/Users/ajcra/Desktop/north/ssl");
	server->run();
	server->getIOService()->run();
}
