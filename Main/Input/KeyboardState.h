#pragma once

#include <vector>

typedef std::vector<bool> KeyCollection;

class KeyboardState
{
public:
	KeyboardState();

	void setKey(unsigned key, bool isDown);
	bool isKeyDown(unsigned key) const;

protected:
	KeyCollection keys;
};