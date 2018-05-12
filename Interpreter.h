#ifndef PILEDSTRIP_SERVER_PROTOCOL_H
#define PILEDSTRIP_SERVER_PROTOCOL_H

#include <cstdint>
#include <utility>
#include <vector>
#include <memory>

#include "LedInterface.h"

class Interpreter
{
	public:
		explicit Interpreter(std::shared_ptr<LedInterface> ledInterface)
			: ledInterface(std::move(ledInterface))
		{
		}

		virtual uint32_t getBufferSize() = 0;
		virtual void interpretData(uint8_t* data, size_t length) = 0;

	protected:
		std::shared_ptr<LedInterface> ledInterface;
};

#endif
