#pragma once

#include "GameTypes.h"
#include "IGameInput.h"
#include "IGameOutput.h"

class GameEngine
{
public:
	virtual ~GameEngine();

	void setUpdateRate(double rate);
	double getUpdateRate() const;

	bool getIsRunning() const;
		
	void run();
	
protected:

protected:
	GameEngine(IGameInput *input, IGameOutput *output);

	void setIsRunning(bool isRunning = true);

	virtual bool initialize(IGameInput *input, IGameOutput *output) { return true; };
	virtual void shutdown() { };

	virtual void update(double frameTime) { };
	virtual void updateInput(const IGameInput *input) { };
	virtual void updateOutput(IGameOutput *output, double frameTime) { };

	virtual void updateWindow(int width, int height) { };

	void checkWindowSize();

private:
	IGameInput * m_Input;
	IGameOutput * m_Output;
	bool m_IsRunning;
	int m_ScreenHeight;
	int m_ScreenWidth;
	double m_MaxFrameTime;
};

