#include "RpiManyInterface.h"

RpiManyInterface::RpiManyInterface(const std::vector<StripConfig>& stripConfigs)
	: LedInterface(stripConfigs)
	, rpiMany(maxLedCount, static_cast<int>(stripConfigs.size()))
	, buffer(totalLedCount * 3)
	, posLut(totalLedCount)
{
	uint32_t lastEnd = 0;

	for (StripConfig c : stripConfigs) {
		row.push_back(buffer.data() + lastEnd * 3);

		for (int i = 0; i < c.ledCount; i++) {
			posLut[lastEnd + i] = lastEnd + (c.flip ? c.ledCount - i - 1 : i);
		}

		lastEnd += c.ledCount;
	}
}

void RpiManyInterface::set(uint16_t pos, uint8_t red, uint8_t green, uint8_t blue)
{
	pos = posLut[pos];
	buffer[pos * 3 + 0] = red;
	buffer[pos * 3 + 1] = green;
	buffer[pos * 3 + 2] = blue;
}

void RpiManyInterface::transmit()
{
	rpiMany(row.data());
}
