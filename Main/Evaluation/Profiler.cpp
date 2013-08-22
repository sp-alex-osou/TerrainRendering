#include "Profiler.h"
#include "..\Game.h"


void GPUPerfLogging(GPA_Logging_Type messageType, const char* message)
{
	Console::out << message << Console::endl;
}


void Profiler::init(ID3D11Device* graphicsDevice)
{
	GPA_RegisterLoggingCallback(GPA_LOGGING_ALL, &GPUPerfLogging);

	GPA_Initialize();
	GPA_OpenContext(graphicsDevice);

	GPA_GetCounterIndex("PrimitivesIn", &primitivesInCounterID);
	GPA_GetCounterIndex("PAPixelsPerTriangle", &pixelsPerTriangleCounterID);

	GPA_EnableCounter(primitivesInCounterID);
	GPA_EnableCounter(pixelsPerTriangleCounterID);

	GPA_GetPassCount(&numPasses);
}


void Profiler::release()
{
	GPA_CloseContext();
	GPA_Destroy();
}


Profiler::Result Profiler::getResult(ID3D11Device* graphicsDevice, Game* game)
{
	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	GPA_BeginSession(&sessionID);

	for (unsigned i = 0; i < numPasses; ++i)
	{
		GPA_BeginPass();
		GPA_BeginSample(0);

		game->drawTerrain(graphicsDevice);

		GPA_EndSample();
		GPA_EndPass();
	}

	GPA_EndSession();

	immediateContext->Release();

	bool readyResult;

	do GPA_IsSessionReady(&readyResult, sessionID); 
	while (!readyResult);

	do GPA_IsSampleReady(&readyResult, sessionID, 0);
	while (!readyResult);

	double primitivesIn;
	double pixelsPerTriangle;

	GPA_GetSampleFloat64(sessionID, 0, primitivesInCounterID, &primitivesIn);
	GPA_GetSampleFloat64(sessionID, 0, pixelsPerTriangleCounterID, &pixelsPerTriangle);

	Result result;

	result.primitiveCount = (unsigned)primitivesIn;
	result.pixelsPerTriangle = (float)pixelsPerTriangle;

	return result;
}