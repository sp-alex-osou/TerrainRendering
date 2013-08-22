#include "Test.h"
#include "..\Game.h"

#include <Windows.h>

// 1024x1024 A, Roughness 0-2x
//float Test::minPixelPerTriangle[][25] = 
//{
//	{ 1.09f, 1.30f, 1.45f, 1.53f, 1.65f, 1.75f, 1.85f, 1.95f, 2.10f, 2.23f, 2.34f, 2.47f, 2.58f, 2.76f, 2.93f, 3.24f, 3.38f, 3.73f, 4.13f, 4.55f, 5.49f, 7.00f, 10.0f, 20.0f, 200.00f },
//	{ 0.00f, 0.25f, 0.28f, 0.33f, 0.39f, 0.50f, 0.55f, 0.60f, 0.66f, 0.72f, 0.78f, 0.84f, 0.93f, 1.01f, 1.07f, 1.20f, 1.25f, 1.36f, 1.50f, 1.74f, 2.06f, 2.88f, 3.50f, 5.0f, 200.00f },
//	{ 0.00f, 0.02f, 0.03f, 0.04f, 0.05f, 0.08f, 0.11f, 0.14f, 0.18f, 0.23f, 0.27f, 0.29f, 0.34f, 0.38f, 0.43f, 0.49f, 0.53f, 0.63f, 0.73f, 0.86f, 1.09f, 1.60f, 2.50f, 5.0f, 200.00f }
//};


// 1024x1024 B & C, Roughness 0-2x
//float Test::minPixelPerTriangle[][25] =
//{
//	{ 3.49f, 4.04f, 4.43f, 4.65f, 5.24f, 5.84f, 6.44f, 7.02f, 7.92f, 8.62f, 9.60f, 10.68f, 12.13f, 13.33f, 14.86f, 18.48f, 21.38f, 24.48f, 31.38f, 42.76f, 63.61f, 100.00f, 300.00f, 1000.0f, 100000000.0f },
//	{ 0.00f, 1.00f, 1.19f, 1.34f, 1.50f, 1.67f, 1.97f, 2.19f, 2.43f, 2.69f, 2.98f, 03.46f, 03.81f, 04.19f, 04.82f, 05.28f, 06.05f, 07.21f, 08.91f, 10.94f, 14.97f, 024.13f, 045.57f, 0090.8f, 100000000.0f }
//};


// 4096x4096 A, Roughness 0-2x
//float Test::minPixelPerTriangle[][25] = 
//{
//	0.000f, 0.360f, 0.380f, 0.395f, 0.410f, 0.430f, 0.450f, 0.480f, 0.510f, 0.550f, 0.590f, 0.630f, 0.660f, 0.690f, 0.750f, 0.830f, 0.910f, 0.950f, 1.030f, 1.180f, 1.530f, 2.480f, 3.000f, 4.100f, 1000.0f,
//	0.000f, 0.020f, 0.025f, 0.030f, 0.040f, 0.050f, 0.060f, 0.070f, 0.080f, 0.090f, 0.100f, 0.110f, 0.120f, 0.130f, 0.140f, 0.150f, 0.170f, 0.190f, 0.220f, 0.270f, 0.340f, 0.400f, 0.500f, 0.910f, 1000.0f,
//	0.000f, 0.001f, 0.002f, 0.003f, 0.004f, 0.005f, 0.007f, 0.009f, 0.010f, 0.011f, 0.013f, 0.016f, 0.020f, 0.024f, 0.030f, 0.034f, 0.040f, 0.050f, 0.060f, 0.078f, 0.106f, 0.140f, 0.183f, 0.322f, 1000.0f
//};


// 4096x4096 B & C, Roughness 0-2x
//float Test::minPixelPerTriangle[][25] =
//{
//	{ 0.87f, 1.00f, 1.12f, 1.19f, 1.34f, 1.41f, 1.50f, 1.58f, 1.67f, 1.97f, 2.19f, 2.43f, 2.83f, 3.29f, 3.81f, 4.60f, 5.53f, 7.21f, 10.94f, 18.75f, 35.16f, 72.00f, 133.00f, 3000.0f, 1000000000.0f },
//	{ 0.00f, 0.07f, 0.12f, 0.16f, 0.19f, 0.24f, 0.28f, 0.32f, 0.37f, 0.43f, 0.49f, 0.56f, 0.68f, 0.78f, 0.94f, 1.19f, 1.58f, 2.43f, 05.05f, 08.00, 11.85f, 24.00f, 062.00f, 0160.0f, 1000000000.0f }
//};


// 16384x16384 A
//float Test::minPixelPerTriangle[][25] =
//{
//	{ 0.00f, 0.03f, 0.04f, 0.05f, 0.06f, 0.07f, 0.08f, 0.10f, 0.12f, 0.14f, 0.16f, 0.18f, 0.20f, 0.23f, 0.26f, 0.30f, 0.32f, 0.35f, 0.40f, 0.49f, 0.60f, 0.88f, 1.26f, 3.46f, 20.0f }
//};


// 16384x16384 B & C
float Test::minPixelPerTriangle[][25] =
{
	{ 0.00f, 0.05f, 0.08f, 0.11f, 0.14f, 0.16f, 0.18f, 0.20f, 0.23f, 0.26f, 0.29f, 0.32f, 0.36f, 0.40f, 0.44f, 0.48f, 0.52f, 0.58f, 0.65f, 0.80f, 1.10f, 1.60f, 3.0f, 8.0f, 10000000000.0f }
};



// 65536x65536 A
//float Test::minPixelPerTriangle[][25] =
//{
//	{ 0.00f, 0.002f, 0.003f, 0.004f, 0.005f, 0.007f, 0.008f, 0.009f, 0.012f, 0.014f, 0.016f, 0.018f, 0.020f, 0.022f, 0.025f, 0.028f, 0.031f, 0.034f, 0.042f, 0.052f, 0.063f, 0.083f, 0.151f, 0.690f, 10000000000000.0f }
//};


// 65536x65536 B & C
//float Test::minPixelPerTriangle[][25] =
//{
//	{ 0.00f, 0.004f, 0.005f, 0.007f, 0.009f, 0.012f, 0.016f, 0.020f, 0.024f, 0.029f, 0.034f, 0.040f, 0.046f, 0.052f, 0.060f, 0.070f, 0.085f, 0.100f, 0.120f, 0.160f, 0.240f, 0.350f, 0.560f, 2.00f, 10000000000000.0f }
//};


void Test::init()
{
	fpsPrimCountActive = false;
	accPrimCountActive = false;
	primCountValuesActive = false;
	fcPrimCountActive = false;
	minPrimCountActive = false;

	fpsPrimCountBuffer.resize(25);
}


void Test::draw(ID3D11Device* graphicsDevice, Game* game)
{
	if (fpsPrimCountActive)
		fpsPrimCountPass(graphicsDevice, game);

	if (accPrimCountActive)
		accPrimCount(graphicsDevice, game);

	if (primCountValuesActive)
		primCountValues(graphicsDevice, game);

	if (fcPrimCountActive)
		fcPrimCount(graphicsDevice, game);

	if (minPrimCountActive)
		minPrimCount(graphicsDevice, game);
}


void Test::primCountValues()
{
	Console::out << "Primitive Count Values Test started" << Console::endl;

	primCountValuesActive = true;
}


void Test::fpsPrimCount()
{
	Console::out << "Primitive Count FPS Test started" << Console::endl;

	fpsPrimCountActive = true;
	numPass = 0;
	prevTickCount = 0;
}


void Test::accPrimCount()
{
	Console::out << "Primitive Count Accuracy Test started" << Console::endl;

	accPrimCountActive = true;
}


void Test::fcPrimCount()
{
	Console::out << "Primitive Count Frustum Culling Test started" << Console::endl;

	fcPrimCountActive = true;
}

void Test::minPrimCount()
{
	Console::out << "Primitive Count Minimum Test started" << Console::endl;

	minPrimCountActive = true;
}


void Test::primCountValues(ID3D11Device* graphicsDevice, Game* game)
{
	std::vector<PrimCountValue> primCountValueBuffer(1000);

	for (unsigned i = 0; i < primCountValueBuffer.size(); ++i)
	{
		Console::out << "Pass " << i + 1 << "/" << primCountValueBuffer.size() << Console::endl;

		//float value = std::pow(i / 500.0f, 6.0f);
		float value = std::pow(i / 100.0f, 6.0f);

		game->terrain->setMinPixelPerTriangle(value);

		PrimCountValue& element = primCountValueBuffer[i];
		element.value = value;
		element.primCount = game->profiler->getResult(graphicsDevice, game).primitiveCount;
	}

	primCountValuesActive = false;

	std::ofstream file;
	file.open("PrimCountValues.csv", std::ios::out);
	printInfo(file, game);

	for (unsigned i = 0; i < primCountValueBuffer.size(); ++i)
		file << primCountValueBuffer[i].primCount << ";" << primCountValueBuffer[i].value << std::endl;

	file.close();

	Console::out << "Primitive Count Values Test finished" << Console::endl;
}


void Test::fcPrimCount(ID3D11Device* graphicsDevice, Game* game)
{
	std::vector<PrimCountFC> fcPrimCountBuffer(25);

	unsigned index = 0;
	//unsigned index = (game->terrain->getRoughnessEnabled()) ? (unsigned)game->terrain->getRoughnessModificator() : 0;

	Terrain* terrain = game->terrain;

	bool gpuFrustumCullingEnabled = terrain->getGPUFrustumCullingEnabled();

	for (unsigned i = 0; i < fcPrimCountBuffer.size(); ++i)
	{
		Console::out << "Pass " << i << "/" << fcPrimCountBuffer.size() << Console::endl;

		terrain->setMinPixelPerTriangle(minPixelPerTriangle[index][i]);

		PrimCountFC& element = fcPrimCountBuffer[i];

		terrain->setGPUFrustumCullingEnabled(false);
		element.primCount1 = game->profiler->getResult(graphicsDevice, game).primitiveCount;

		terrain->setGPUFrustumCullingEnabled(true);
		element.primCount2 = game->profiler->getResult(graphicsDevice, game).primitiveCount;
	}

	terrain->setGPUFrustumCullingEnabled(gpuFrustumCullingEnabled);

	fcPrimCountActive = false;

	std::ofstream file;
	file.open("PrimCountFC.csv", std::ios::out);
	printInfo(file, game);

	for (unsigned i = 0; i < fcPrimCountBuffer.size(); ++i)
		file << fcPrimCountBuffer[i].primCount1 << ";" << fcPrimCountBuffer[i].primCount2 << std::endl;

	file.close();

	Console::out << "Primitive Count Accuracy Test finished" << Console::endl;
}


void Test::accPrimCount(ID3D11Device* graphicsDevice, Game* game)
{
	std::vector<PrimCountAcc> accPrimCountBuffer(25);

	unsigned index = 0;
	//unsigned index = (game->terrain->getRoughnessEnabled()) ? (unsigned)game->terrain->getRoughnessModificator() : 0;

	for (unsigned i = 0; i < accPrimCountBuffer.size(); ++i)
	{
		Console::out << "Pass " << i + 1 << "/" << accPrimCountBuffer.size() << Console::endl;

		game->terrain->setMinPixelPerTriangle(minPixelPerTriangle[index][i]);

		PrimCountAcc& element = accPrimCountBuffer[i];
		element.accuracy = game->accuracy->getAccuracy(graphicsDevice, game);
		//element.accuracy = game->accuracy->getAccuracy(graphicsDevice, game, true);

		//game->terrain->setGPUFrustumCullingEnabled(false);
		element.primCount1 = game->profiler->getResult(graphicsDevice, game).primitiveCount;

		//game->terrain->setGPUFrustumCullingEnabled(true);
		//element.primCount2 = game->profiler->getResult(graphicsDevice, game).primitiveCount;
	}

	accPrimCountActive = false;

	std::ofstream file;
	file.open("PrimCountAcc.csv", std::ios::out);
	printInfo(file, game);

	for (unsigned i = 0; i < accPrimCountBuffer.size(); ++i)
		file << accPrimCountBuffer[i].primCount1 << ";" << accPrimCountBuffer[i].accuracy << std::endl;
		//file << accPrimCountBuffer[i].primCount1 << ";" << accPrimCountBuffer[i].primCount2 << ";" << accPrimCountBuffer[i].accuracy << std::endl;

	file.close();

	Console::out << "Primitive Count Accuracy Test finished" << Console::endl;
}


void Test::fpsPrimCountPass(ID3D11Device* graphicsDevice, Game* game)
{
	unsigned index = 0;
	//unsigned index = (game->terrain->getRoughnessEnabled()) ? (unsigned)game->terrain->getRoughnessModificator() : 0;

	if (numPass < fpsPrimCountBuffer.size())
	{
		unsigned long tickCount = GetTickCount();

		if (!prevTickCount)
		{
			Console::out << "Pass " << numPass + 1 << "/" << fpsPrimCountBuffer.size() << Console::endl;
			game->terrain->setMinPixelPerTriangle(minPixelPerTriangle[index][numPass]);
			prevTickCount = tickCount;
		}
		else if (tickCount - prevTickCount > 1500)
		{
			PrimCountFPS& element = fpsPrimCountBuffer[numPass++];
			element.fps = DXUTGetFPS();
			element.primCount = game->profiler->getResult(graphicsDevice, game).primitiveCount;
			prevTickCount = 0;
		}
	}
	else
		fpsPrimCountFinished(game);
}


void Test::fpsPrimCountFinished(Game* game)
{
	fpsPrimCountActive = false;

	std::ofstream file;
	file.open("PrimCountFPS.csv", std::ios::out);
	printInfo(file, game);

	for (unsigned i = 0; i < fpsPrimCountBuffer.size(); ++i)
		file << fpsPrimCountBuffer[i].primCount << ";" << (unsigned)fpsPrimCountBuffer[i].fps << std::endl;

	file.close();

	Console::out << "Primitive Count FPS Test finished" << Console::endl;
}


void Test::minPrimCount(ID3D11Device* graphicsDevice, Game* game)
{
	Terrain* terrain = game->terrain;

	terrain->setMinPixelPerTriangle(10000000000.0f);

	Console::out << game->profiler->getResult(graphicsDevice, game).primitiveCount << Console::endl;

	minPrimCountActive = false;

	Console::out << "Primitive Count Minimum Test finished" << Console::endl;
}


void Test::printInfo(std::ofstream& file, Game* game)
{
	//Terrain* terrain = game->terrain;

	//file << "Terrain Resolution: " << terrain->getWidth() << "x" << terrain->getLength() << std::endl;
	//file << "Camera Position: " << game->camera->getPosition() << std::endl;
	//file << "Roughness Enabled: " << terrain->getRoughnessEnabled() << std::endl;
	//file << "CPU Frustum Culling: " << terrain->getCPUFrustumCullingEnabled() << std::endl;
	//file << "GPU Frustum Culling: " << terrain->getGPUFrustumCullingEnabled() << std::endl;
	//file << "Uniform Patch Size: " << terrain->getUniformPatchSizeEnabled() << std::endl;
	//file << "Patch Size: " << terrain->getPatchSize() << std::endl;
	//file << "Node Size: " << terrain->getNodeSize() << std::endl;
	//file << "Bumpiness: " << terrain->getBumpiness() << std::endl;
	//file << std::endl;
}
