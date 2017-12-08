#include <iostream>
#include <boost/beast.hpp>
#include "Server.h"

int main() {
	Server* server = new Server();
	server->run(5650);
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	}
}
