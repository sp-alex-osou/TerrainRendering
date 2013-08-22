#pragma once

#include <vector>
#include <fstream>
#include <ctime>

#include <D3D11.h>

class Game;

class Test
{
	struct PrimCountFPS
	{
		unsigned primCount;
		float fps;
	};

	struct PrimCountAcc
	{
		unsigned primCount1;
		unsigned primCount2;
		float accuracy;
	};

	struct PrimCountFC
	{
		unsigned primCount1;
		unsigned primCount2;
	};

	struct PrimCountValue
	{
		unsigned primCount;
		float value;
	};

public:
	void init();
	void draw(ID3D11Device* graphicsDevice, Game* game);

	void fpsPrimCount();
	void accPrimCount();
	void fcPrimCount();
	void primCountValues();
	void minPrimCount();

private:
	void fpsPrimCountPass(ID3D11Device* graphicsDevice, Game* game);
	void fpsPrimCountFinished(Game* game);

	void accPrimCount(ID3D11Device* graphicsDevice, Game* game);
	void primCountValues(ID3D11Device* graphicsDevice, Game* game);
	void fcPrimCount(ID3D11Device* graphicsDevice, Game* game);
	void minPrimCount(ID3D11Device* graphicsDevice, Game* game);

	void printInfo(std::ofstream& file, Game* game);

	std::vector<PrimCountFPS> fpsPrimCountBuffer;

	bool fpsPrimCountActive;
	bool accPrimCountActive;
	bool fcPrimCountActive;
	bool primCountValuesActive;
	bool minPrimCountActive;

	unsigned numPass;
	unsigned long prevTickCount;

	static float minPixelPerTriangle[][25];
};