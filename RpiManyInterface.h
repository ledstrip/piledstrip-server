#ifndef PILEDSTRIP_SERVER_RPIMANYINTERFACE_H
#define PILEDSTRIP_SERVER_RPIMANYINTERFACE_H

#include <vector>

#include "LedInterface.h"
#include "WS2812BOut.hpp"

class RpiManyInterface : public LedInterface
{
	public:
		explicit RpiManyInterface(const std::vector<StripConfig>& stripConfigs);

		void set(uint16_t pos, uint8_t red, uint8_t green, uint8_t blue) override;
		void transmit() override;

	private:
		WS2812BOut rpiMany;
		std::vector<uint8_t> buffer;
		std::vector<uint16_t> posLut;
		std::vector<const uint8_t*> row;
};

#endif
