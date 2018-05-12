#include "LedInterface.h"

#include <algorithm>
#include <numeric>

LedInterface::LedInterface(std::vector<StripConfig> stripConfigs)
	: maxLedCount(std::max_element(stripConfigs.begin(), stripConfigs.end(), [](const StripConfig& c1, const StripConfig& c2) {
	return c1.ledCount < c2.ledCount;
})->ledCount)
	, totalLedCount(
		std::accumulate(stripConfigs.begin(), stripConfigs.end(), maxLedCount - stripConfigs.back().ledCount, [](uint16_t acc, const StripConfig& c) {
			return acc + c.ledCount;
		}))
	, stripConfig(std::move(stripConfigs))
{
}
