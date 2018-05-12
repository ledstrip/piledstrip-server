#ifndef PILEDSTRIP_SERVER_ESPINTERPRETER_H
#define PILEDSTRIP_SERVER_ESPINTERPRETER_H

#include "Interpreter.h"

class EspInterpreter : public Interpreter
{
	public:
		explicit EspInterpreter(const std::shared_ptr<LedInterface>& ledInterface);

		uint32_t getBufferSize() override;
		void interpretData(uint8_t* data, size_t length) override;
};

#endif
