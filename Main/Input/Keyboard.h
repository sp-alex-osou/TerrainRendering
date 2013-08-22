#pragma once

#include "InputDevice.h"
#include "KeyboardState.h"

class Keyboard
{
public:
	static void init();
	static void update();
	static void setKey(unsigned key, bool isDown);

	static bool isKeyDown(unsigned key);
	static bool isKeyPress(unsigned key);
	static bool isKeyRelease(unsigned key);

protected:
	static bool isKeyDown(unsigned key, int state);

private:
	static InputDevice<KeyboardState> keyboard;
};