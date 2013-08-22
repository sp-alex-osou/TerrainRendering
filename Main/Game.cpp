#include "Game.h"

Game::Game()
{
	camera = new Camera();
	terrain = new Terrain();
	hud = new HUD();
}


Game::~Game()
{
	delete camera;
	delete terrain;
	delete hud;
}

 
void Game::init(ID3D11Device* graphicsDevice)
{
	Mouse::init();
	Keyboard::init();

	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	D3D11_RASTERIZER_DESC desc;

	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.ScissorEnable = false;
	desc.AntialiasedLineEnable = true;
	desc.MultisampleEnable = true;

	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_WIREFRAME;
	graphicsDevice->CreateRasterizerState(&desc, &rsWireframe);

	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_SOLID;
	graphicsDevice->CreateRasterizerState(&desc, &rsSolid);

	rsCurrent = rsSolid;

	immediateContext->Release();

	freeCamera = true;

	camera->init();
	terrain->init(graphicsDevice);
	hud->init(graphicsDevice, this);

	camera->setPosition(Vector3::up * 100.0f);

	for (unsigned i = 0; i < 3; ++i)
	{
		white[i] = 1;
		black[i] = 0;
	}

	white[3] = black[3] = 1;

	clearColor = black;
}


void Game::release()
{
	camera->release();
	terrain->release();
	hud->release();

	rsSolid->Release();
	rsWireframe->Release();
}


void Game::update(const GameTime& gameTime)
{
	Mouse::update();
	Keyboard::update();

	camera->update(gameTime);
	terrain->update(camera->getPosition(), camera->getViewFrustum());

	if (!freeCamera)
	{
		Vector3 position = camera->getPosition();
		position.y = terrain->getHeight(position) + 5.0f;
		camera->setPosition(position);
	}

	if (Keyboard::isKeyPress('M'))
	{
		Mouse::setVisible(!Mouse::isVisible());
		Mouse::setPosition(Window::getCenter());
	}

	if (Keyboard::isKeyPress('H'))
		hud->setDialogVisible(!hud->isDialogVisible());

	if (Keyboard::isKeyPress('T'))
		hud->setTextVisible(!hud->isTextVisible());

	if (Keyboard::isKeyPress('1'))
		clearColor = black;

	if (Keyboard::isKeyPress('2'))
		clearColor = white;

	if (Keyboard::isKeyPress('3'))
	{
		camera->setPosition(Vector3(-200, 330, -200));
		camera->look(Vector3(0.6f, -0.5f, 0.6f));
	}

	if (Keyboard::isKeyPress('4'))
	{
		camera->setPosition(Vector3(512, 5, 512));
		camera->look(Vector3(1, 0, 0));
	}

	if (Keyboard::isKeyPress('5'))
	{
		camera->setPosition(Vector3(512, 5, 512));
		camera->look(Vector3(-1, 0, 0));
	}
}


void Game::draw(ID3D11Device* graphicsDevice, const GameTime& gameTime)
{
	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	immediateContext->ClearRenderTargetView(renderTargetView, clearColor);
	immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

	immediateContext->RSSetState(rsCurrent);

	terrain->setFakeView(camera->isLocked() ? camera->getLockedView() : camera->getView());
	terrain->draw(graphicsDevice, Matrix::identity, camera->getView(), camera->getProjection());

	hud->draw(graphicsDevice, this);

	immediateContext->Release();
}


void Game::onResizedSwapChain(ID3D11Device* graphicsDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	unsigned width = pBackBufferSurfaceDesc->Width;
	unsigned height = pBackBufferSurfaceDesc->Height;

	camera->setAspectRatio((float)width / (float)height);
	terrain->setScreenSize(width, height);

	hud->onResizedSwapChain(graphicsDevice, pBackBufferSurfaceDesc);
}


void Game::onReleasingSwapChain()
{
	hud->onReleasingSwapChain();
}


void Game::drawTerrain(ID3D11Device* graphicsDevice)
{
	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	immediateContext->ClearRenderTargetView(renderTargetView, clearColor);
	immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

	terrain->draw(graphicsDevice, Matrix::identity, camera->getView(), camera->getProjection());

	immediateContext->Release();
}
