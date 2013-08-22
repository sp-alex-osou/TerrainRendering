#pragma once

#include <D3D11.h>

#include <GPUPerfAPI.h>

class Game;

class Profiler
{
public:
	struct Result
	{
		unsigned primitiveCount;
		float pixelsPerTriangle;
	};

	void init(ID3D11Device* graphicsDevice);
	void release();

	Result getResult(ID3D11Device* graphicsDevice, Game* game);

private:
	unsigned sessionID;

	unsigned primitivesInCounterID;
	unsigned pixelsPerTriangleCounterID;

	unsigned numPasses;

	bool waitingForResult;
};