#include "TcpServer.h"

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>

TcpConnection::TcpConnection(TcpServer* tcpServer, boost::asio::ip::tcp::socket socket)
	: tcpServer(tcpServer)
	, socket(std::move(socket))
	, buffer(tcpServer->bufferSize)
{
	startRead();
}

void TcpConnection::startRead()
{
	boost::asio::async_read(socket, boost::asio::buffer(buffer), [this](const boost::system::error_code& error, std::size_t bytesReceived) {
		if (error) {
			if (error == boost::asio::error::eof) {
				std::cerr << "Connection closed" << std::endl;
			} else {
				std::cerr << "Error on receive: " << error.message() << std::endl;
			}

			tcpServer->closeConnection(this);
			return;
		}

		if (bytesReceived > 0) {
			std::cout << bytesReceived << " bytes received" << std::endl;
			tcpServer->dataReceived("source", buffer.data(), bytesReceived);
		}

		startRead();
	});
}

TcpServer::TcpServer(boost::asio::io_service& ioService, uint16_t port, uint32_t bufferSize)
	: bufferSize(bufferSize)
	, acceptor(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	, newSocket(ioService)
{
	std::cout << "Listening on TCP port " << port << std::endl;
	startAccept();
}

void TcpServer::startAccept()
{
	acceptor.async_accept(newSocket, [this](const boost::system::error_code& error) {
		if (!error) {
			std::cout << "New client connected from " << newSocket.remote_endpoint().address() << std::endl;
			connections.emplace_back(this, std::move(newSocket));
		}

		startAccept();
	});
}

void TcpServer::closeConnection(TcpConnection* connection)
{
	connections.erase(std::remove_if(connections.begin(), connections.end(), [connection](TcpConnection& testConn) {
		return connection == &testConn;
	}), connections.end());
}
