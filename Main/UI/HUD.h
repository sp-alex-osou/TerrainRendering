#pragma once

#include "..\Types\Types.h"
#include "..\Terrain\Terrain.h"

#include <sstream>
#include <vector>

#include <DXUT.h>
#include <DXUTgui.h>
#include <DXUTmisc.h>
#include <DXUTCamera.h>
#include <DXUTSettingsDlg.h>
#include <SDKmisc.h>
#include <SDKmesh.h>

extern CDXUTDialogResourceManager dialogResourceManager;
extern CDXUTTextHelper textHelper;
extern CDXUTDialog dialog;

class Game;

class HUD
{
	enum ControlIDs
	{
		CameraMovementSlider,
		CameraMovementStatic,
		RoughnessModificatorSlider,
		RoughnessModificatorStatic,
		MinPixelPerTriangleSlider,
		MinPixelPerTriangleStatic,
		TextureRepeatSlider,
		TextureRepeatStatic,
		BumpinessSlider,
		BumpinessStatic,
		WireframeCheckbox,
		UpdateLODCheckbox,
		CPUFrustumCullingCheckbox,
		GPUFrustumCullingCheckbox,
		SnapToTerrainCheckbox,
		ColormapCheckbox,
		LightingCheckbox,
		HeightTextureCheckbox,
		SlopeTextureCheckbox,
		RoughnessCheckbox,
		AntiShimmeringCheckbox,
		ShowNodesCheckbox,
		UniformPatchSizeCheckbox,
		BruteForceCheckbox
	};

public:
	HUD();

	void init(ID3D11Device* graphicsDevice, Game* game);
	void draw(ID3D11Device* graphicsDevice, Game* game);
	void release();

	void setDialogVisible(bool visible) { dialog.SetVisible(visible); }
	bool isDialogVisible() const { return dialog.GetVisible(); }

	void setTextVisible(bool visible) { textVisible = visible; }
	bool isTextVisible() const { return textVisible; }

	void onReleasingSwapChain();
	void onResizedSwapChain(ID3D11Device* graphicsDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

private:
	void static CALLBACK onGuiEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);

	bool textVisible;
};
