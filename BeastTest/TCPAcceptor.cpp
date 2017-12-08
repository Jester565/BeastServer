#include "TCPAcceptor.h"
#include "ServicePool.h"
#include "TCPConnection.h"
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio/placeholders.hpp>

using namespace boost::asio::ip;

TCPAcceptor::TCPAcceptor(boost::shared_ptr<ServicePool> servicePool)
	:servicePool(servicePool)
{
}

void TCPAcceptor::detatch(uint16_t port)
{
	acceptor = boost::make_shared<tcp::acceptor>(servicePool->getNextIOService(), tcp::endpoint(boost::asio::ip::tcp::v6(), port));
	runAccept();
}

void TCPAcceptor::close()
{
	if (acceptor != nullptr) {
		acceptor->close();
	}
	if (tempSocket != nullptr) {
		boost::system::error_code ec;
		tempSocket->shutdown(tcp::socket::shutdown_both, ec);
		tempSocket->close();
	}
}


TCPAcceptor::~TCPAcceptor()
{
}

void TCPAcceptor::asyncAcceptHandler(const boost::system::error_code & ec)
{
	if (ec) {
		std::cerr << "Error" << std::endl;
		return;
	}
	boost::shared_ptr<TCPConnection> tcpConnection = boost::make_shared<TCPConnection>(tempSocket);
	tcpConnection->start();
	cons.push_back(tcpConnection);
	runAccept();
}

void TCPAcceptor::runAccept()
{
	tempSocket = boost::make_shared<boost::asio::ip::tcp::socket>(servicePool->getNextIOService());
	acceptor->async_accept(*tempSocket, boost::bind(&TCPAcceptor::asyncAcceptHandler, shared_from_this(), boost::asio::placeholders::error));
}
