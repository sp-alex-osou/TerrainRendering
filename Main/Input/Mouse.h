#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>

#include "InputDevice.h"
#include "MouseState.h"

#include "..\Helpers\Helpers.h"

class Mouse
{
public:
	typedef std::vector<Vector2> Buffer;

	static void init();
	static void update();

	static void setState(const MouseState& mouseState);

	static bool isLeftButtonDown();
	static bool isRightButtonDown();

	static bool isLeftButtonClick();
	static bool isRightButtonClick();

	static bool isLeftButtonClickEnd();
	static bool isRightButtonClickEnd();

	static bool isVisible();
	static const Point& getPosition();
	static int getMouseWheelDelta();

	static void setPosition(const Point& position);
	static void setVisible(bool visible);

	static Vector2 getMovement();

protected:
	static bool visible;

private:
	static InputDevice<MouseState> mouse;

	static Buffer buffer;
	static unsigned bufferIndex;

	static float smoothing;
	static bool firstUpdate;
};