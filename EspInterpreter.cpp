#include <cstdint>
#include "EspInterpreter.h"

// Protocol constants
const size_t DATA_OFFSET = 3;
const size_t DATA_STRIDE = 3;
const size_t RED_OFFSET = 1;
const size_t GREEN_OFFSET = 0;
const size_t BLUE_OFFSET = 2;

EspInterpreter::EspInterpreter(const std::shared_ptr<LedInterface>& ledInterface)
	: Interpreter(ledInterface)
{
}

uint32_t EspInterpreter::getBufferSize()
{
	return static_cast<uint32_t>(ledInterface->getLedCount() * 3 + DATA_OFFSET);
}

void EspInterpreter::interpretData(uint8_t* data, size_t length)
{
	size_t ledCountLength = (length - DATA_OFFSET) / 3;
	uint16_t ledCount = ledInterface->getLedCount();
	if (ledCountLength < ledCount) {
		ledCount = static_cast<uint16_t>(ledCountLength);
	}

	for (uint16_t pos = 0; pos < ledCount; pos++) {
		size_t offset = DATA_OFFSET + pos * DATA_STRIDE;
		ledInterface->set(pos, data[offset + RED_OFFSET], data[offset + GREEN_OFFSET], data[offset + BLUE_OFFSET]);
	}

	ledInterface->transmit();
}
