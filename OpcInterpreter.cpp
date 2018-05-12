#include "OpcInterpreter.h"

#include <ctime>
#include <iostream>
#include <utility>
#include <memory>

// Protocol constants
const size_t LED_COUNT_HIGH_BYTE_OFFSET = 2;
const size_t LED_COUNT_LOW_BYTE_OFFSET = 3;
const size_t DATA_OFFSET = 4;
const size_t DATA_STRIDE = 3;
const size_t RED_OFFSET = 0;
const size_t GREEN_OFFSET = 1;
const size_t BLUE_OFFSET = 2;

OpcInterpreter::OpcInterpreter(std::shared_ptr<LedInterface> ledInterface)
	: Interpreter(std::move(ledInterface))
{
}

uint32_t OpcInterpreter::getBufferSize()
{
	return static_cast<uint32_t>(ledInterface->getLedCount() * 3 + DATA_OFFSET);
}

void OpcInterpreter::interpretData(uint8_t* data, size_t length)
{
	if (length >= DATA_OFFSET) {
		uint16_t ledCountReceived = (data[LED_COUNT_HIGH_BYTE_OFFSET] << 8) | data[LED_COUNT_LOW_BYTE_OFFSET];

		if (ledCountReceived != ledInterface->getLedCount()) {
			std::cout << "Warning: header contains data for " << ledCountReceived << " LEDs but server is configured to "
					  << ledInterface->getLedCount() << " LEDs" << std::endl;
		} else {
			std::cout << "Received LED count: " << ledCountReceived << std::endl;
		}

		if (ledCountReceived * 3 + DATA_OFFSET != length) {
			std::cout << "Warning: invalid data length " << length << " bytes for " << ledCountReceived << " LEDs" << std::endl;
		} else {
			std::cout << "Acceptable data length" << std::endl;
		}

		size_t ledCountLength = (length - DATA_OFFSET) / 3;
		uint32_t ledCount = std::min(static_cast<uint32_t>(ledCountReceived), ledInterface->getLedCount());
		if (ledCountLength < ledCount) {
			ledCount = static_cast<uint16_t>(ledCountLength);
		}

		for (uint16_t pos = 0; pos < ledCount; pos++) {
			size_t offset = DATA_OFFSET + pos * DATA_STRIDE;

			if (pos == 0) {
				std::cout << "Setting pixel color at position 0 to " << int(data[offset + RED_OFFSET]) << ", " << int(data[offset + GREEN_OFFSET])
						  << ", " << int(data[offset + BLUE_OFFSET]) << std::endl;
			}

			ledInterface->set(pos, data[offset + RED_OFFSET], data[offset + GREEN_OFFSET], data[offset + BLUE_OFFSET]);
		}

		std::cout << "Displaying..." << std::endl;
		ledInterface->transmit();
	}
}
