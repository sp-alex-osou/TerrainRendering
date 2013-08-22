#include "KeyboardState.h"


KeyboardState::KeyboardState() : keys(KeyCollection(256, false))
{
}

void KeyboardState::setKey(unsigned key, bool isDown)
{
	keys[key] = isDown;
}


bool KeyboardState::isKeyDown(unsigned key) const
{
	return keys[key];
}