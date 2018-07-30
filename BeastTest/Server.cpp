#include "Server.h"
#include "TCPAcceptor.h"
#include "HttpClient.h"
#include "ClientManager.h"
#include "EventManager.h"
#include <boost/make_shared.hpp>

Server::Server(uint16_t port, const std::string& chainFile, const std::string& keyFile, const std::string& verifyPath)
{
	ioService = new boost::asio::io_service();
	sslContext = boost::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
	sslContext->set_options(boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::single_dh_use);
	try {
		sslContext->use_certificate_chain_file(chainFile);
		sslContext->use_private_key_file(keyFile, boost::asio::ssl::context::pem);
		sslContext->set_verify_mode(boost::asio::ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert);
		sslContext->load_verify_file(verifyPath);
	}
	catch (std::exception& ex) {
		std::cerr << "ERROR WHEN CREATING SSL CONTEXT: " << ex.what() << std::endl;
	}
	tcpAcceptor = boost::shared_ptr<TCPAcceptor>(new TCPAcceptor(port, ioService, sslContext, 
		(raw_connect_handler)std::bind(&Server::rawConnectHandler, this, std::placeholders::_1)));
	clientManager = new ClientManager();
	evtManager = new EventManager();
}

void Server::run()
{
	tcpAcceptor->run();
}

void Server::rawConnectHandler(ssl_socket socket)
{
	std::cout << "CONNECTED" << std::endl;
	client_ptr client = boost::shared_ptr<HttpClient>(new HttpClient(socket, 
		(msg_handler)std::bind(&Server::msgHandler, this, std::placeholders::_1, std::placeholders::_2),
		(disconnect_handler)std::bind(&Server::disconnectHandler, this, std::placeholders::_1)));
	clientManager->addClient(client);
	client->start();
	evtManager->runConnect(client);
}

void Server::msgHandler(client_ptr client, req_ptr req)
{
	std::cout << "MESSAGE" << std::endl;
	std::cout << *req << std::endl;

	evtManager->runMessage(client, req);
	/*
	auto resp = CreateResponse();
	resp->result(200);
	resp->keep_alive(true);
	resp->set(http::field::server, "Beast");
	resp->set(http::field::content_type, "text/plain");
	resp->body() = (std::string)req->body();
	client->send(resp);
	*/
}

void Server::disconnectHandler(client_ptr client)
{
	evtManager->runDisconnect(client);
	std::cout << "DISCONNECT" << std::endl;
	clientManager->removeClient(client);
}
