#ifndef PILEDSTRIP_SERVER_SERVER_H
#define PILEDSTRIP_SERVER_SERVER_H

#include <cstdint>

#include <boost/signals2.hpp>

class Server
{
	public:
		boost::signals2::signal<void(const std::string& source, uint8_t* data, size_t length)> dataReceived;
};

#endif
