#include "TCPAcceptor.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio/placeholders.hpp>

using namespace boost::asio::ip;

TCPAcceptor::TCPAcceptor(uint16_t port, boost::asio::io_service* ioService, boost::shared_ptr<boost::asio::ssl::context> sslContext, raw_connect_handler conHandler)
	:ioService(ioService), sslContext(sslContext), conHandler(conHandler)
{
	acceptor = boost::make_shared<tcp::acceptor>(*ioService, tcp::endpoint(boost::asio::ip::tcp::v6(), port));
}

void TCPAcceptor::run()
{
	runAccept();
}

void TCPAcceptor::close()
{
	if (acceptor != nullptr) {
		acceptor->close();
	}
	if (tempSocket != nullptr) {
		boost::system::error_code ec;
		tempSocket->lowest_layer().shutdown(tcp::socket::shutdown_both, ec);
		tempSocket->lowest_layer().close();
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
	conHandler(tempSocket);
	runAccept();
}

void TCPAcceptor::runAccept()
{
	tempSocket = boost::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>(*ioService, *sslContext);
	acceptor->async_accept(tempSocket->lowest_layer(), boost::bind(&TCPAcceptor::asyncAcceptHandler, shared_from_this(), boost::asio::placeholders::error));
}
