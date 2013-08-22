#include "MouseState.h"


MouseState::MouseState(bool leftButtonDown, bool rightButtonDown, int mouseWheelDelta, const Point& position)
	: leftButtonDown(leftButtonDown), rightButtonDown(rightButtonDown), mouseWheelDelta(mouseWheelDelta), position(position)
{

}
