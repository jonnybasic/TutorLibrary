#pragma once

#include "IGameInput.h"

class ANSIGameInput 
	: public IGameInput
{
public:
	ANSIGameInput();
	virtual ~ANSIGameInput();

	virtual bool initialize();
	virtual void shutdown();

	virtual byte getInput() const;
	virtual bool tryGetInput(byte &input) const;

	unsigned long long getLastError() const {
		return m_LastError;
	};

protected:
	virtual bool getConsoleMode(unsigned long *mode);
	virtual bool setConsoleMode(unsigned long mode);

private:
	void * m_Handle;
	bool m_IsEnabled;
	bool m_InLineMode;
	int m_ViewportHeight;
	int m_ViewportWidth;
	unsigned long m_ModeDefault;
	unsigned long long m_LastError;
};

