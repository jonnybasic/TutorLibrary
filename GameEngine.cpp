#include "GameEngine.h"


GameEngine::GameEngine(
	IGameInput *input,
	IGameOutput *output)
	: m_Input(input),
	m_Outut(output) 
{
}


GameEngine::~GameEngine()
{
	delete m_Input;
	delete m_Outut;
}


void GameEngine::run()
{

}