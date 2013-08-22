#include "HUD.h"

#include <strsafe.h>

#include "..\Game.h"

HUD::HUD()
{
}


void HUD::init(ID3D11Device* graphicsDevice, Game* game)
{
	dialog.Init(&dialogResourceManager);
	dialog.SetCallback(HUD::onGuiEvent, game);

	ID3D11DeviceContext* deviceContext = DXUTGetD3D11DeviceContext();

	Terrain* terrain = game->terrain;
	Camera* camera = game->camera;

	textVisible = true;

	textHelper = CDXUTTextHelper(graphicsDevice, deviceContext, &dialogResourceManager, 15);

	unsigned width = 200;
	unsigned height = 20;
	unsigned delta = height * 2;
	unsigned i = 1;
	unsigned y = 0;

	WCHAR sz[200];

	float movementSpeed = camera->getMovementSpeed();
	StringCchPrintf(sz, 200, L"Camera Movement: %.1f", movementSpeed);
	dialog.AddStatic(CameraMovementStatic, sz, 0, y, width, height);
	dialog.AddSlider(CameraMovementSlider, 0, y + height, width, height, 1, 500, (int)movementSpeed);
	y += delta;

	float bumpiness = terrain->getBumpiness();
	StringCchPrintf(sz, 200, L"Bumpiness: %.1f", bumpiness);
	dialog.AddStatic(BumpinessStatic, sz, 0, y, width, height);
	dialog.AddSlider(BumpinessSlider, 0, y + height, width, height, 0, 2000, (int)bumpiness);
	y += delta;

	float minPixelPerTriangle = terrain->getMinPixelPerTriangle();
	StringCchPrintf(sz, 200, L"Triangle Size: %.1f", minPixelPerTriangle);
	dialog.AddStatic(MinPixelPerTriangleStatic, sz, 0, y, width, height);
	dialog.AddSlider(MinPixelPerTriangleSlider, 0, y + height, width, height, 0, 20000, (int)(minPixelPerTriangle * 100.0f));
	y += delta;

	float roughnessModificator = terrain->getRoughnessModificator();
	StringCchPrintf(sz, 200, L"Roughness Modificator: %.2f", roughnessModificator);
	dialog.AddStatic(RoughnessModificatorStatic, sz, 0, y, width, height);
	dialog.AddSlider(RoughnessModificatorSlider, 0, y + height, width, height, 0, 1000, (int)(roughnessModificator * 100.0f));
	y += delta;
	
	float textureRepeat = terrain->getColormapRepeat();
	StringCchPrintf(sz, 200, L"Texture Repeat: %.1f", textureRepeat);
	dialog.AddStatic(TextureRepeatStatic, sz, 0, y, width, height);
	dialog.AddSlider(TextureRepeatSlider, 0, y + height, width, height, 1, 1024, (int)textureRepeat);
	y += delta;

	delta = (unsigned)(height * 1.2f);

	dialog.AddCheckBox(WireframeCheckbox, L"Wireframe (TAB)", 0, y + delta * i++, width, height, game->isWireframe(), VK_TAB); 
	dialog.AddCheckBox(UpdateLODCheckbox, L"Update LOD", 0, y + delta * i++, width, height, !camera->isLocked()); 
	dialog.AddCheckBox(CPUFrustumCullingCheckbox, L"CPU Frustum Culling", 0, y + delta * i++, width, height, terrain->getCPUFrustumCullingEnabled()); 
	dialog.AddCheckBox(GPUFrustumCullingCheckbox, L"GPU Frustum Culling", 0, y + delta * i++, width, height, terrain->getGPUFrustumCullingEnabled()); 
	dialog.AddCheckBox(SnapToTerrainCheckbox, L"Snap to Terrain", 0, y + delta * i++, width, height, !game->isFreeCamera()); 
	dialog.AddCheckBox(ColormapCheckbox, L"Colormap", 0, y + delta * i++, width, height, terrain->getColormapEnabled());
	dialog.AddCheckBox(LightingCheckbox, L"Lighting", 0, y + delta * i++, width, height, terrain->getLightingEnabled()); 
	dialog.AddCheckBox(HeightTextureCheckbox, L"Height-based Texturing", 0, y + delta * i++, width, height, terrain->getHeightTextureEnabled());
	dialog.AddCheckBox(SlopeTextureCheckbox, L"Slope-based Texturing", 0, y + delta * i++, width, height, terrain->getSlopeTextureEnabled());
	dialog.AddCheckBox(RoughnessCheckbox, L"Enable Roughness", 0, y + delta * i++, width, height, terrain->getRoughnessEnabled());
	dialog.AddCheckBox(AntiShimmeringCheckbox, L"Anti Shimmering", 0, y + delta * i++, width, height, terrain->getAntiShimmeringEnabled());
	dialog.AddCheckBox(ShowNodesCheckbox, L"Show Nodes", 0, y + delta * i++, width, height, terrain->getShowNodesEnabled());
	dialog.AddCheckBox(UniformPatchSizeCheckbox, L"Uniform Patch Size", 0, y + delta * i++, width, height, terrain->getUniformPatchSizeEnabled());
	dialog.AddCheckBox(BruteForceCheckbox, L"Brute Force", 0, y + delta * i++, width, height, terrain->getBruteForceEnabled());

	dialogResourceManager.OnD3D11CreateDevice(graphicsDevice, deviceContext);
}


void CALLBACK HUD::onGuiEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	WCHAR sz[200];

	Game* game = (Game*)pUserContext;
	Terrain* terrain = game->terrain;
	Camera* camera = game->camera;

	switch (nControlID)
	{
		case CameraMovementSlider:
			{
				CDXUTSlider* slider = (CDXUTSlider*)pControl;
				int movement = slider->GetValue();

				camera->setMovementSpeed((float)movement);
				StringCchPrintf(sz, 200, L"Camera Movement: %d", movement);
				dialog.GetStatic(CameraMovementStatic)->SetText(sz);
			}
			break;

		case BumpinessSlider:
			{
				CDXUTSlider* slider = (CDXUTSlider*)pControl;
				int bumpiness = slider->GetValue();

				terrain->setBumpiness((float)bumpiness);
				StringCchPrintf(sz, 200, L"Bumpiness: %d", bumpiness);
				dialog.GetStatic(BumpinessStatic)->SetText(sz);
			}
			break;

		case MinPixelPerTriangleSlider:
			{
				CDXUTSlider* slider = (CDXUTSlider*)pControl;
				float minPixelPerTriangle = slider->GetValue() / 100.0f;

				terrain->setMinPixelPerTriangle(minPixelPerTriangle);
				StringCchPrintf(sz, 200, L"Triangle Size: %.2f", minPixelPerTriangle);
				dialog.GetStatic(MinPixelPerTriangleStatic)->SetText(sz);
			}
			break;


		case RoughnessModificatorSlider:
			{
				CDXUTSlider* slider = (CDXUTSlider*)pControl;
				float roughnessModificator = slider->GetValue() / 100.0f;

				terrain->setRoughnessModificator(roughnessModificator);
				StringCchPrintf(sz, 200, L"Roughness Modificator: %.2f", roughnessModificator);
				dialog.GetStatic(RoughnessModificatorStatic)->SetText(sz);
			}
			break;

		case TextureRepeatSlider:
			{
				CDXUTSlider* slider = (CDXUTSlider*)pControl;
				int textureRepeat = slider->GetValue();

				terrain->setColormapRepeat((float)textureRepeat);
				StringCchPrintf(sz, 200, L"Texture Repeat: %d", textureRepeat);
				dialog.GetStatic(TextureRepeatStatic)->SetText(sz);
			}
			break;

		case WireframeCheckbox:
			{
				game->toggleWireframe();
			}
			break;

		case UpdateLODCheckbox:
			{
				camera->toggleLock();
			}
			break;

		case CPUFrustumCullingCheckbox:
			{
				terrain->setCPUFrustumCullingEnabled(!terrain->getCPUFrustumCullingEnabled());
			}
			break;

		case GPUFrustumCullingCheckbox:
			{
				terrain->setGPUFrustumCullingEnabled(!terrain->getGPUFrustumCullingEnabled());
			}
			break;

		case SnapToTerrainCheckbox:
			{
				game->toggleFreeCamera();
			}
			break;

		case LightingCheckbox:
			{
				terrain->setLightingEnabled(!terrain->getLightingEnabled());
			}
			break;

		case HeightTextureCheckbox:
			{
				terrain->setHeightTextureEnabled(!terrain->getHeightTextureEnabled());
			}
			break;

		case SlopeTextureCheckbox:
			{
				terrain->setSlopeTextureEnabled(!terrain->getSlopeTextureEnabled());
			}
			break;

		case RoughnessCheckbox:
			{
				terrain->setRoughnessEnabled(!terrain->getRoughnessEnabled());
			}
			break;

		case AntiShimmeringCheckbox:
			{
				terrain->setAntiShimmeringEnabled(!terrain->getAntiShimmeringEnabled());
			}
			break;

		case ShowNodesCheckbox:
			{
				terrain->setShowNodesEnabled(!terrain->getShowNodesEnabled());
			}
			break;

		case UniformPatchSizeCheckbox:
			{
				terrain->setUniformPatchSizeEnabled(!terrain->getUniformPatchSizeEnabled());
			}
			break;

		case BruteForceCheckbox:
			{
				terrain->setBruteForceEnabled(!terrain->getBruteForceEnabled());
			}
			break;

		case ColormapCheckbox:
			{
				terrain->setColormapEnabled(!terrain->getColormapEnabled());
			}
			break;
	}
}


void HUD::release()
{
	dialogResourceManager.OnD3D11DestroyDevice();
}


void HUD::draw(ID3D11Device* graphicsDevice, Game* game)
{
	dialog.OnRender(DXUTGetElapsedTime());

	if (!textVisible)
		return;

	Terrain* terrain = game->terrain;

	textHelper.Begin();

	textHelper.SetInsertionPos(5, 5);
	textHelper.SetForegroundColor(D3DXCOLOR(1, 1, 0, 1));
	textHelper.DrawFormattedTextLine(DXUTGetFrameStats(true));
	textHelper.DrawFormattedTextLine(DXUTGetDeviceStats());
	textHelper.DrawFormattedTextLine(L"Terrain Resolution: %d x %d", terrain->getWidth(), terrain->getLength());
	textHelper.DrawFormattedTextLine(L"Node Size: %d x %d", terrain->getNodeSize(), terrain->getNodeSize());

	textHelper.End();
}


void HUD::onResizedSwapChain(ID3D11Device* graphicsDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	dialogResourceManager.OnD3D11ResizedSwapChain(graphicsDevice, pBackBufferSurfaceDesc);
	dialog.SetLocation(pBackBufferSurfaceDesc->Width - 220, 20);
	dialog.SetSize(200, 500);
}


void HUD::onReleasingSwapChain()
{
	dialogResourceManager.OnD3D11ReleasingSwapChain();
}