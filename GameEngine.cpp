#include <chrono>
#include <windows.h>

#include "GameEngine.h"

using std::chrono::system_clock;
using std::chrono::duration;

GameEngine::GameEngine(
	IGameInput *input,
	IGameOutput *output)
	: m_Input(input),
	m_Output(output),
	m_IsRunning(false),
	m_MaxFrameTime(1.0 / 30.0)
{
}


GameEngine::~GameEngine()
{
	delete m_Input;
	delete m_Output;
}

void GameEngine::setUpdateRate(double rate) 
{
	m_MaxFrameTime = 1.0 / rate;
}

double GameEngine::getUpdateRate() const
{
	return 1.0 / m_MaxFrameTime;
}

bool GameEngine::getIsRunning() const
{
	return m_IsRunning;
}

void GameEngine::setIsRunning(bool isRunning)
{
	m_IsRunning = isRunning;
}

void GameEngine::run()
{
	if (!m_Input->initialize())
	{
		// TODO: set error
		return;
	}
	if (!m_Output->initialize())
	{
		// TODO: set error
		return;
	}
	if (!initialize(m_Input, m_Output))
	{
		// TODO: set error
		return;
	}

	// start
	m_IsRunning = true;

	auto screenWidth = 0;
	auto screenHeight = 0;

	auto lastFrameTime = system_clock::now();
	while (m_IsRunning)
	{
		// Perform update
		const auto frameStartTime = system_clock::now();
		duration<double> frameTime(lastFrameTime - frameStartTime);

		// Check for screen change
		m_Output->getViewport(screenWidth, screenHeight);
		updateWindow(screenWidth, screenHeight);

		// Collect input
		updateInput(m_Input);
		// Check for exit
		if (!m_IsRunning) 
		{
			break;
		}

		// Main update
		update(frameTime.count());
		const auto updateEndTime = system_clock::now();
		const duration<double> updateTime(updateEndTime - frameStartTime);

		// Render
		frameTime = lastFrameTime - updateEndTime;
		updateOutput(m_Output, frameTime.count());
		const auto renderEndTime = system_clock::now();
		const duration<double> renderTime(renderEndTime - updateEndTime);

		frameTime = frameStartTime - renderEndTime;
		fflush(stdout);

		// TODO: get frame stats
		const auto syncTime = static_cast<unsigned long>((m_MaxFrameTime - frameTime.count()) * 1000.0);
		if (syncTime > 15u)
		{
			Sleep(syncTime);
		}
	}
	// Shutdown output;
	m_Output->shutdown();
	// Shutdown input
	m_Input->shutdown();
	// Shutdown
	shutdown();
}
