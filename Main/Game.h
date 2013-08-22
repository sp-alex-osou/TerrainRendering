#pragma once

#include <D3D11.h>
#include <D3DX11.h>

#include <d3dx11effect.h>

#include <ctime>

#include "Input\Input.h"
#include "Helpers\Helpers.h"
#include "Types\Types.h"
#include "Camera\Camera.h"
#include "UI\HUD.h"
#include "Evaluation\Profiler.h"
#include "Evaluation\Accuracy.h"
#include "Evaluation\Test.h"
#include "Terrain\Terrain.h"

class Game
{
	friend class HUD;
	friend class Accuracy;
	friend class Profiler;
	friend class Test;

public:
	Game();
	~Game();

	void init(ID3D11Device* graphicsDevice);
	void release();

	void update(const GameTime& gameTime);
	void draw(ID3D11Device* graphicsDevice, const GameTime& gameTime);

	void setRenderTargetView(ID3D11RenderTargetView* renderTargetView) { this->renderTargetView = renderTargetView; }
	void setDepthStencilView(ID3D11DepthStencilView* depthStencilView) { this->depthStencilView = depthStencilView; }

	void onReleasingSwapChain();
	void onResizedSwapChain(ID3D11Device* graphicsDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

	bool isFreeCamera() const { return freeCamera; }
	void toggleFreeCamera() { freeCamera = !freeCamera; }

	bool isWireframe() const { return rsCurrent == rsWireframe; }
	void toggleWireframe() { rsCurrent = (isWireframe()) ? rsSolid : rsWireframe; }

private:
	void drawTerrain(ID3D11Device* graphicsDevice);

	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;

	Terrain* terrain;
	Camera* camera;
	HUD* hud;
	Profiler* profiler;
	Accuracy* accuracy;
	Test* test;

	float white[4];
	float black[4];

	float* clearColor;

	bool freeCamera;

	ID3D11RasterizerState* rsSolid;
	ID3D11RasterizerState* rsWireframe;
	ID3D11RasterizerState* rsCurrent;
};
