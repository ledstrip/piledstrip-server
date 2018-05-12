#ifndef PILEDSTRIP_SERVER_UDPSERVER_H
#define PILEDSTRIP_SERVER_UDPSERVER_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>

#include "Server.h"

class UdpServer : public Server
{
	public:
		explicit UdpServer(boost::asio::io_service& ioService, uint16_t port);

	private:
		boost::asio::ip::udp::socket socket;
		boost::asio::ip::udp::endpoint remoteEndpoint;
		std::array<unsigned char, 65507> buffer; // Maximum UDP payload

		void startReceive();
		void handleReceiveFrom(const boost::system::error_code& error, size_t bytesReceived);
};

#endif
