#ifndef PILEDSTRIP_SERVER_LEDINTERFACE_H
#define PILEDSTRIP_SERVER_LEDINTERFACE_H

#include <cstdint>
#include <vector>

#include "StripConfig.h"

class LedInterface
{
	public:
		explicit LedInterface(std::vector<StripConfig> stripConfigs);

		virtual void set(uint16_t pos, uint8_t red, uint8_t green, uint8_t blue) = 0;
		virtual void transmit() = 0;

		virtual uint32_t getLedCount()
		{
			return totalLedCount;
		}

	protected:
		const uint16_t maxLedCount;
		const uint32_t totalLedCount;
		const std::vector<StripConfig> stripConfig;
};

#endif
