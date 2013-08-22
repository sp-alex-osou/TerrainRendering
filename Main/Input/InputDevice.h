#pragma once

template <class T>
class InputDevice
{
public:
	InputDevice();

	void update();

	T& getState(int state);

	static const int prevState = 0;
	static const int currState = 1;
	static const int nextState = 2;

private:
	T states[3];
	int stateIndices[3];
};


template <class T>
InputDevice<T>::InputDevice()
{
	for (int i = 0; i < 3; ++i)
		stateIndices[i] = i;
}


template <class T>
void InputDevice<T>::update()
{
	for (int i = 0; i < 3; ++i)
		stateIndices[i] = (stateIndices[i] + 1) % 3;

	getState(nextState) = getState(currState);
}


template <class T>
T& InputDevice<T>::getState(int state)
{
	return states[stateIndices[state]];
}