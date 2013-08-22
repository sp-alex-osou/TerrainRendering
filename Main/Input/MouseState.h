#pragma once

#include "..\Types\Types.h"

class MouseState
{
public:
	MouseState(bool leftButtonDown = false, bool rightButtonDown = false, int mouseWheelDelta = 0, const Point& position = Point());

	bool leftButtonDown;
	bool rightButtonDown;
	int mouseWheelDelta;
	Point position;
};