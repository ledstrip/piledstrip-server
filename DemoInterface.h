#ifndef PILEDSTRIP_SERVER_DEMOINTERFACE_H
#define PILEDSTRIP_SERVER_DEMOINTERFACE_H

#include "LedInterface.h"

class DemoInterface : public LedInterface
{
	public:
		explicit DemoInterface(const std::vector<StripConfig>& stripConfigs);

		void set(uint16_t pos, uint8_t red, uint8_t green, uint8_t blue) override;
		void transmit() override;
};

#endif
