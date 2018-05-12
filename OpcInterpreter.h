#ifndef PILEDSTRIP_SERVER_OPCINTERPRETER_H
#define PILEDSTRIP_SERVER_OPCINTERPRETER_H

#include "Interpreter.h"

class OpcInterpreter : public Interpreter
{
	public:
		explicit OpcInterpreter(std::shared_ptr<LedInterface> ledInterface);

		uint32_t getBufferSize() override;
		void interpretData(uint8_t* data, size_t length) override;
};

#endif
