#include "DemoInterface.h"

#include <iostream>

DemoInterface::DemoInterface(const std::vector<StripConfig>& stripConfigs)
	: LedInterface(stripConfigs)
{
}

void DemoInterface::set(uint16_t pos, uint8_t red, uint8_t green, uint8_t blue)
{
	std::cout << "DEMO INTERFACE: setting pos " << pos << " to (" << red << ", " << green << ", " << blue << ")";
}

void DemoInterface::transmit()
{
	std::cout << "DEMO INTERFACE: transmitting data";
}
