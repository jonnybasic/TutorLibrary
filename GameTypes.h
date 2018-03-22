#pragma once

class IGameOutput;

class IGameInput;

class GameEngine;

typedef unsigned char byte;

struct Color
{
	byte r;
	byte g;
	byte b;

	Color(byte r = 0, byte g = 0, byte b = 0)
		: r(r), g(g), b(b) { }
};
