#include "Server.h"
#include "TCPAcceptor.h"
#include "HttpClient.h"
#include "ClientManager.h"

Server::Server(uint16_t port)
{
	ioService = new boost::asio::io_service();
	tcpAcceptor = boost::shared_ptr<TCPAcceptor>(new TCPAcceptor(port, ioService, (raw_connect_handler)std::bind(&Server::rawConnectHandler, this, std::placeholders::_1)));
	clientManager = new ClientManager();
}

void Server::run()
{
	tcpAcceptor->run();
}

void Server::rawConnectHandler(socket_ptr socket)
{
	std::cout << "CONNECTED" << std::endl;
	client_ptr client = boost::shared_ptr<HttpClient>(new HttpClient(socket, 
		(msg_handler)std::bind(&Server::msgHandler, this, std::placeholders::_1, std::placeholders::_2),
		(disconnect_handler)std::bind(&Server::disconnectHandler, this, std::placeholders::_1)));
	clientManager->addClient(client);
	client->start();
}

void Server::msgHandler(client_ptr client, req_ptr req)
{
	std::cout << "MESSAGE" << std::endl;
	std::cout << *req << std::endl;
	
	auto resp = CreateResponse();
	resp->result(200);
	resp->keep_alive(true);
	resp->set(http::field::server, "Beast");
	resp->set(http::field::content_type, "text/plain");
	resp->body() = (std::string)req->body();
	client->send(resp);
}

void Server::disconnectHandler(client_ptr client)
{
	std::cout << "DISCONNECT" << std::endl;
	clientManager->removeClient(client);
}
