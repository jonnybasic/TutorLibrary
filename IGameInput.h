#pragma once

#include "GameTypes.h"

class IGameInput
{
public:
	virtual bool initialize() = 0;
	virtual void shutdown() = 0;

	virtual byte getInput() const = 0;
	virtual bool tryGetInput(byte &input) const = 0;
};