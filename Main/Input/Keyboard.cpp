#include "Keyboard.h"


InputDevice<KeyboardState> Keyboard::keyboard;


void Keyboard::init()
{
}


void Keyboard::update()
{
	keyboard.update();
}


void Keyboard::setKey(unsigned key, bool isDown)
{
	keyboard.getState(keyboard.nextState).setKey(key, isDown);
}


bool Keyboard::isKeyDown(unsigned key)
{
	return isKeyDown(key, keyboard.currState);
}


bool Keyboard::isKeyPress(unsigned key)
{
	return isKeyDown(key, keyboard.currState) && !isKeyDown(key, keyboard.prevState);
}


bool Keyboard::isKeyRelease(unsigned key)
{
	return !isKeyDown(key, keyboard.currState) && isKeyDown(key, keyboard.prevState);
}


bool Keyboard::isKeyDown(unsigned key, int state)
{
	return keyboard.getState(state).isKeyDown(key);
}