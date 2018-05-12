#include "UdpServer.h"

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>

UdpServer::UdpServer(boost::asio::io_service& ioService, uint16_t port)
	: socket(ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
	, buffer{}
{
	std::cout << "Listening on UDP port " << port << std::endl;
	startReceive();
}

void UdpServer::startReceive()
{
	socket.async_receive_from(boost::asio::buffer(buffer), remoteEndpoint, boost::bind(&UdpServer::handleReceiveFrom, this, _1, _2));
}

void UdpServer::handleReceiveFrom(const boost::system::error_code& error, size_t bytesReceived)
{
	if (error) {
		std::cerr << "Error: " << error.message() << std::endl;
	}

	if (bytesReceived > 0) {
		std::cout << bytesReceived << " bytes received" << std::endl;
		dataReceived("source", buffer.data(), bytesReceived);
	}

	startReceive();
}
