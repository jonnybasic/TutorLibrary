#include <windows.h>
#include <stdio.h>

#include "ANSIGameOutput.h"

#define ESC "\x1b"
#define CSI "\x1b["
#define RESET CSI "0m"
#define SCP CSI "s"
#define RCP CSI "u"
#define ENTER_BUFFER CSI "?1049h"
#define EXIT_BUFFER CSI "?1049l"
#define ENTER_LINE ESC "(0"
#define EXIT_LINE ESC "(B"
#define BLINK_ON CSI "?12h"
#define BLINK_OFF CSI "?12l"
#define CURSOR_ON CSI "?25h"
#define CURSOR_OFF CSI "?25l"
#define MOVE_FMT CSI "%i;%iH"
#define FORE_FMT CSI "38;2;%i;%i;%im"
#define BACK_FMT CSI "48;2;%i;%i;%im"
#define CLS CSI "2J"

#define DEC_BR "\x6a"
#define DEC_TR "\x6b"
#define DEC_TL "\x6c"
#define DEC_BL "\x6d"
#define DEC_C  "\x6e"
#define DEC_H  "\x71"
#define DEC_ML "\x74"
#define DEC_MR "\x75"
#define DEC_MT "\x76"
#define DEC_MB "\x77"
#define DEC_V  "\x78"

ANSIGameOutput::ANSIGameOutput()
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

ANSIGameOutput::~ANSIGameOutput()
{
	if (m_IsEnabled)
	{
		shutdown();
	}
}

bool ANSIGameOutput::setConsoleMode(unsigned long mode)
{
	if (!m_Handle) return false;
	if (!SetConsoleMode(m_Handle, mode))
	{
		m_LastError = GetLastError();
		return false;
	}
	return true;
}

bool ANSIGameOutput::getConsoleMode(unsigned long *mode)
{
	if (!m_Handle) return false;
	if (!GetConsoleMode(m_Handle, mode))
	{
		m_LastError = GetLastError();
		return false;
	}
	return true;
}

bool ANSIGameOutput::initialize()
{
	if (!m_Handle)
	{
		return false;
	}
	if (getConsoleMode(&m_ModeDefault))
	{
		// new mode
		const auto mode = m_ModeDefault 
			| ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		// enable virtual console
		setConsoleMode(mode);
		m_IsEnabled = true;
		// Enter the alternate buffer
		printf(ENTER_BUFFER);
		// get screen size
		CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
		if (GetConsoleScreenBufferInfo(m_Handle, &csbiInfo))
		{
			m_ViewportHeight = csbiInfo.dwSize.Y;
			m_ViewportWidth = csbiInfo.dwSize.X;
		}
		// Turn off cursor
		printf(CURSOR_OFF);
		return true;
	}
	else
	{
		return false;
	}
}

void ANSIGameOutput::shutdown()
{
	// Exit the alternate buffer
	printf(EXIT_BUFFER);
	// disable virtual console
	setConsoleMode(m_ModeDefault);
	m_IsEnabled = false;
}

void ANSIGameOutput::setViewport(int w, int h)
{

}

void ANSIGameOutput::getViewport(int &w, int &h)
{
	h = m_ViewportHeight;
	w = m_ViewportWidth;
}

void ANSIGameOutput::setForeColor(const Color &color)
{
	printf(FORE_FMT, color.r, color.g, color.b);
}

void ANSIGameOutput::setBackColor(const Color &color)
{
	printf(BACK_FMT, color.r, color.g, color.b);
}

bool ANSIGameOutput::getInLineMode() const
{
	return m_InLineMode;
}

void ANSIGameOutput::setInLineMode(bool inLineMode)
{
	if (inLineMode && !m_InLineMode)
	{
		printf(ENTER_LINE);
		m_InLineMode = true;
	}
	else if (!inLineMode && m_InLineMode)
	{
		printf(EXIT_LINE);
		m_InLineMode = false;
	}
}

void ANSIGameOutput::moveTo(int x, int y)
{
	printf(MOVE_FMT, y + 1, x + 1);
}

void ANSIGameOutput::save()
{
	printf(SCP);
}

void ANSIGameOutput::restore()
{
	printf(RCP);
}

void ANSIGameOutput::print(const char *message)
{
	printf(message);
}

void ANSIGameOutput::drawBox(int x, int y, int w, int h)
{
	setInLineMode(true);
	save();
	moveTo(x, y);
	printLine(LineCode::CornerTopLeft);
	for (auto i = 0; i < w - 2; i++)
	{
		printLine(LineCode::HorizontalBar);
	}
	printLine(LineCode::CornerTopRight);
	for (auto i = y + 1; i < (y + h) - 1; i++)
	{
		moveTo(x, i);
		printLine(LineCode::VerticalBar);
		moveTo((x + w) - 1, i);
		printLine(LineCode::VerticalBar);
	}
	moveTo(x, (y + h) - 1);
	printLine(LineCode::CornerBottomLeft);
	for (auto i = 0; i < w - 2; i++)
	{
		printLine(LineCode::HorizontalBar);
	}
	printLine(LineCode::CornerBottomRight);
	restore();
	setInLineMode(false);
}

void ANSIGameOutput::printLine(LineCode code)
{
	switch (code)
	{
	case LineCode::CornerBottomLeft:
		printf(DEC_BL);
		break;
	case LineCode::CornerBottomRight:
		printf(DEC_BR);
		break;
	case LineCode::CornerTopLeft:
		printf(DEC_TL);
		break;
	case LineCode::CornerTopRight:
		printf(DEC_TR);
		break;
	case LineCode::Cross:
		printf(DEC_C);
		break;
	case LineCode::HorizontalBar:
		printf(DEC_H);
		break;
	case LineCode::VerticalBar:
		printf(DEC_V);
		break;
	case LineCode::EdgeBottom:
		printf(DEC_MB);
		break;
	case LineCode::EdgeLeft:
		printf(DEC_ML);
		break;
	case LineCode::EdgeRight:
		printf(DEC_MR);
		break;
	case LineCode::EdgeTop:
		printf(DEC_MT);
		break;
	}
}

void ANSIGameOutput::reset()
{
	printf(RESET);
}

void ANSIGameOutput::clear()
{
	// Clear screen
	printf(CLS);
}