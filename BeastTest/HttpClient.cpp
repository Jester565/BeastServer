#include "HttpClient.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::beast;
HttpClient::HttpClient(ssl_socket socket, msg_handler msgHandler, disconnect_handler disHandler)
	:socket(socket), msgHandler(msgHandler), disHandler(disHandler)
{
	
}

void HttpClient::start()
{
	socket->set_verify_callback(std::bind(&HttpClient::asyncVerifyHandler, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	socket->async_handshake(boost::asio::ssl::stream_base::server, boost::bind(&HttpClient::asyncHandshakeHandler, shared_from_this(), boost::asio::placeholders::error));
}

void HttpClient::send(boost::shared_ptr<http::response<http::string_body>> resp)
{
	respStore = resp;
	resp->prepare_payload();
	http::async_write(*socket, *resp, 
		boost::bind(&HttpClient::asyncSendHandler, 
			shared_from_this(), 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred, 
			resp->need_eof()));
}

HttpClient::~HttpClient()
{
}

void HttpClient::close()
{
	boost::system::error_code ec;
	socket->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
	if (disHandler != nullptr) {
		disHandler(shared_from_this());
	}
}

void HttpClient::readRequest()
{
	req = CreateRequest();
	std::cout << buffer.size() << std::endl;
	//buffer.consume(buffer.size());
	http::async_read(*socket, buffer, *req,
		boost::bind(&HttpClient::asyncReceiveHandler, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

bool HttpClient::asyncVerifyHandler(bool preverified, boost::asio::ssl::verify_context & verifyCtx)
{
	std::cout << "VERIFY CALLED" << std::endl;
	char subjectName[256];
	X509* cert = X509_STORE_CTX_get_current_cert(verifyCtx.native_handle());
	X509_NAME_oneline(X509_get_subject_name(cert), subjectName, 256);
	subject = subjectName;
	std::cout << "SUBJECT: " << subject << std::endl;
	return preverified;
}

void HttpClient::asyncHandshakeHandler(const boost::system::error_code& error) {
	if (!error)
	{
		std::cout << "SSL Handshake Success!" << std::endl;
		readRequest();
	}
	else
	{
		std::cerr << "Error occured in SSL Handshake: " << error.message() << std::endl;
		std::string hrerr;
		hrerr += boost::lexical_cast<std::string>(ERR_GET_LIB(error.value()));
		hrerr += ", ";
		hrerr += boost::lexical_cast<std::string>(ERR_GET_FUNC(error.value()));
		hrerr += ", ";
		hrerr += boost::lexical_cast<std::string>(ERR_GET_REASON(error.value()));
		hrerr += ", ";
		char buf[128];
		ERR_error_string_n(error.value(), buf, 128);
		hrerr += buf;
		std::cerr << "Human Readable Error Version: " << hrerr << std::endl;
	}
}

void HttpClient::asyncReceiveHandler(const boost::system::error_code & error, unsigned int nBytes)
{
	if (error == http::error::end_of_stream) {
		close();
	}
		

	if (error) {
		std::cout << *req << std::endl;
		std::cout << req->body() << std::endl;
		std::cerr << "Receive Error: " << error.message() << std::endl;
		return;
	}
	/*
	std::cout << "RECEIVED: " << nBytes << std::endl;
	std::cout << req << std::endl;
	auto resp = CreateResponse();
	resp->result(200);
	resp->keep_alive(true);
	resp->set(http::field::server, "Beast");
	resp->set(http::field::content_type, "text/plain");
	resp->body() = (std::string)req->body();
	send(resp);
	*/
	if (msgHandler != nullptr) {
		msgHandler(shared_from_this(), req);
	}
	else {
		std::cout << "DEFAULT MESSAGE HANDLER: " << req << std::endl;
	}
	readRequest();
}

void HttpClient::asyncSendHandler(const boost::system::error_code & error, unsigned int nBytes, bool close)
{
	if (error) {
		std::cerr << "Send Error: " << error.message() << std::endl;
	}
	respStore = nullptr;
}
