#pragma once
#include "Typedef.h"
#include <stdint.h>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>

class TCPAcceptor : public boost::enable_shared_from_this<TCPAcceptor>
{
public:
	TCPAcceptor(uint16_t port, boost::asio::io_service* ioService, raw_connect_handler conHandler = nullptr);

	void run();

	void close();

	boost::shared_ptr<boost::asio::ip::tcp::acceptor> getRawAcceptor() {
		return acceptor;
	}

	~TCPAcceptor();

private:
	void asyncAcceptHandler(const boost::system::error_code& ec);

	void runAccept();

	boost::shared_ptr<boost::asio::ip::tcp::socket> tempSocket;

	boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;

	boost::asio::io_service* ioService;

	raw_connect_handler conHandler;
};

