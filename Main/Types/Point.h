#pragma once

#include "Vector2.h"

struct Point
{
	Point(int x = 0, int y = 0) : x(x), y(y) {}

	operator Vector2() { return Vector2((float)x, (float)y); }

	int x;
	int y;
};