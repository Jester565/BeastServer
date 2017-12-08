#pragma once
#include <stdint.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <boost/enable_shared_from_this.hpp>

class ServicePool;
class TCPConnection;

class TCPAcceptor : public boost::enable_shared_from_this<TCPAcceptor>
{
public:
	TCPAcceptor(boost::shared_ptr<ServicePool> servicePool);

	void detatch(uint16_t port);

	void close();

	~TCPAcceptor();

private:
	void asyncAcceptHandler(const boost::system::error_code& ec);

	void runAccept();

	boost::shared_ptr<boost::asio::ip::tcp::socket> tempSocket;

	boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;

	boost::shared_ptr<ServicePool> servicePool;

	//TEMPORARY
	std::vector<boost::shared_ptr<TCPConnection>> cons;
};

