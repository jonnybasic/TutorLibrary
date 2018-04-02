#pragma once

#include "GameTypes.h"

class IGameOutput
{
public:
	virtual bool initialize() = 0;
	virtual void shutdown() = 0;

	virtual void setViewport(int w, int h) = 0;
	virtual void getViewport(int &w, int &h) const = 0;

	virtual void moveTo(int x, int y) = 0;
	
	virtual void setForeColor(const Color &color) = 0;
	virtual void setBackColor(const Color &color) = 0;

	virtual void print(const char *message) = 0;

	virtual void save() = 0;
	virtual void restore() = 0;

	virtual void clear() = 0;
};