#pragma once

#include "GameTypes.h"
#include "IGameInput.h"
#include "IGameOutput.h"

class GameEngine
{
public:
	GameEngine(IGameInput *input, IGameOutput *output);
	virtual ~GameEngine();

	virtual void run();

protected:
	IGameInput * m_Input;
	IGameOutput * m_Outut;
};

