#include <windows.h>
#include <conio.h>
#include <iostream>

#include "ANSIGameInput.h"

ANSIGameInput::ANSIGameInput()
	: m_Handle(nullptr),
	m_IsEnabled(false),
	m_InLineMode(false),
	m_ModeDefault(0),
	m_LastError(0)
{
	m_Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (m_Handle == INVALID_HANDLE_VALUE)
	{
		m_LastError = GetLastError();
		m_Handle = nullptr;
	}
}


ANSIGameInput::~ANSIGameInput()
{
}

bool ANSIGameInput::setConsoleMode(unsigned long mode)
{
	if (!m_Handle) return false;
	if (!SetConsoleMode(m_Handle, mode))
	{
		m_LastError = GetLastError();
		return false;
	}
	return true;
}

bool ANSIGameInput::getConsoleMode(unsigned long *mode)
{
	if (!m_Handle) return false;
	if (!GetConsoleMode(m_Handle, mode))
	{
		m_LastError = GetLastError();
		return false;
	}
	return true;
}

bool ANSIGameInput::initialize()
{
	if (!m_Handle)
	{
		return false;
	}
	if (getConsoleMode(&m_ModeDefault))
	{
		// new mode
		const auto mode = m_ModeDefault 
			| ENABLE_WINDOW_INPUT 
			| ENABLE_MOUSE_INPUT;
		// enable virtual console
		setConsoleMode(mode);
		m_IsEnabled = true;
		return true;
	}
	else
	{
		return false;
	}
}

void ANSIGameInput::shutdown()
{
	// disable virtual console
	setConsoleMode(m_ModeDefault);
	m_IsEnabled = false;
}

byte ANSIGameInput::getInput() const
{
	return static_cast<byte>(_getch());
}

bool ANSIGameInput::tryGetInput(byte &input) const {
	if (_kbhit())
	{
		input = static_cast<byte>(_getch());
		return true;
	}
	return false;
}

