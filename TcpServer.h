#ifndef PILEDSTRIP_SERVER_TCPSERVER_H
#define PILEDSTRIP_SERVER_TCPSERVER_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "Server.h"

class TcpServer;

class TcpConnection
{
	public:
		explicit TcpConnection(TcpServer* tcpServer, boost::asio::ip::tcp::socket socket);

	private:
		void startRead();

		TcpServer* tcpServer;
		boost::asio::ip::tcp::socket socket;
		std::vector<unsigned char> buffer;
};

class TcpServer : public Server
{
	public:
		const uint32_t bufferSize;

		explicit TcpServer(boost::asio::io_service& ioService, uint16_t port, uint32_t bufferSize);
		void closeConnection(TcpConnection* pConnection);

	private:
		boost::asio::ip::tcp::acceptor acceptor;
		std::list<TcpConnection> connections;
		boost::asio::ip::tcp::socket newSocket;

		void startAccept();
};

#endif
