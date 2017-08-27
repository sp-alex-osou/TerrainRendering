#define NUM_CONTROL_POINTS 1

float4x4 FakeView;

float4x4 World;
float4x4 View;
float4x4 Projection;

float4x4 WorldViewProjection;

Texture2D Heightmap;
Texture2D Normalmap;
Texture2D Infomap;
Texture2D Slopemap;
Texture2D Colormap;
Texture2D Roughnessmap;

Texture2DArray Colormaps;

uint2 ScreenSize;
float2 SlopeRange;

uint TerrainSize;
float TerrainScale;

uint MinPatchSize;
uint MinNodeSize;

float ColormapRepeat;
float MinPixelPerTriangle;
float RoughnessModificator;
float Bumpiness;

bool ColormapEnabled;
bool HeightTextureEnabled;
bool SlopeTextureEnabled;
bool LightingEnabled;
bool FrustumCullingEnabled;
bool RoughnessEnabled;
bool AntiShimmeringEnabled;
bool ShowNodesEnabled;
bool BruteForceEnabled;


SamplerState SamplerLinearWrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


SamplerState SamplerLinearClamp
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};


SamplerState SamplerPointClamp
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};


struct VSControlPoint
{
	float2 Position : Position;
};


struct Node
{
	float2 Position : NodePosition;
	float Size : NodeSize;
};


struct ControlPoint
{
	float2 Position : Position;
	float Size : Size;
};


struct HSConstantOutput
{
	float Edges[4] : SV_TessFactor;
	float Inside[2] : SV_InsideTessFactor;
	float MipMapLevel : MipMapLevel;
	float4 Color : Color;
	//bool Culled;
};


struct DSOutput
{
	float4 Position : SV_Position;
	float2 TexCoord	: TexCoord;
	float4 Color : Color;
	//bool Culled;
};


float2 Project(float4 p)
{
	p = mul(p, Projection);

	return (p.xy / p.w) * ScreenSize;
}


float GetTessellationFactor(float4 p0, float4 p1)
{
	float4x4 WorldView = mul(World, FakeView);

	float4 view0 = mul((p0 + p1) * 0.5f, WorldView);
	float4 view1 = view0;

	view1.x += distance(p0, p1);

	return distance(Project(view0), Project(view1)) / MinPixelPerTriangle;
}


bool3 OutsideFrustum(float4 p, float y)
{
	float4x4 WorldViewProjection = mul(mul(World, FakeView), Projection);

	p.y = y;
	p = mul(p, WorldViewProjection);
	p.x /= abs(p.w);

	return bool3(p.x < -1, p.x > 1, p.w < 0);
}


bool OutsideFrustum(float4 p[4])
{
	bool3 outside = true;	

	for (uint i = 0; i < 4; ++i)
	{
		outside = outside && OutsideFrustum(p[i], 0);
		outside = outside && OutsideFrustum(p[i], Bumpiness);
	}

	return outside.x || outside.y || outside.z;
}


ControlPoint VSDefault(VSControlPoint input, Node node)
{
	ControlPoint cp;

	uint patchesPerNode = MinNodeSize / MinPatchSize;

	node.Size *= TerrainScale;
	node.Position *= TerrainScale;

	cp.Size = node.Size / patchesPerNode;
	cp.Position = node.Position + input.Position * cp.Size;
	
	return cp;
}


HSConstantOutput HSConstantDefault(InputPatch<ControlPoint, NUM_CONTROL_POINTS> input, uint patchID : SV_PrimitiveID)
{
	HSConstantOutput output;

	//output.Culled = false;

	float2 pos = input[0].Position;
	float size = input[0].Size;

	uint lod = log2(size  / (MinPatchSize * TerrainScale));
	float4 p[4];
	uint i;

	p[0] = float4(pos.x + 0 * size, 0, pos.y + 0 * size, 1);
	p[1] = float4(pos.x + 0 * size, 0, pos.y + 1 * size, 1);
	p[2] = float4(pos.x + 1 * size, 0, pos.y + 1 * size, 1);
	p[3] = float4(pos.x + 1 * size, 0, pos.y + 0 * size, 1);

	// Frustum Culling
	if (FrustumCullingEnabled && OutsideFrustum(p))
	{
		for (i = 0; i < 4; ++i)
			output.Edges[i] = -1;

		for (i = 0; i < 2; ++i)
			output.Inside[i] = -1;

		return output;

		//output.Culled = true;
	}

	for (i = 0; i < 4; ++i)
		output.Edges[i] = (BruteForceEnabled || MinPixelPerTriangle == 0.0f) ? 64 : GetTessellationFactor(p[i], p[(i+1)%4]);

	for (i = 0; i < 2; ++i)
		output.Inside[i] = (output.Edges[i] + output.Edges[i+2]) * 0.5f;

	if (RoughnessEnabled && !BruteForceEnabled && MinPixelPerTriangle != 0.0f)
	{
		float2 roughnessUV = pos / TerrainSize;
		float roughness = Roughnessmap.SampleLevel(SamplerPointClamp, roughnessUV, lod);
			
		float edges[4];
		float delta = pow(2, lod) * MinPatchSize * TerrainScale / TerrainSize;

		edges[0] = Roughnessmap.SampleLevel(SamplerPointClamp, float2(roughnessUV + float2(-delta, 0)), lod);
		edges[1] = Roughnessmap.SampleLevel(SamplerPointClamp, float2(roughnessUV + float2(0, +delta)), lod);
		edges[2] = Roughnessmap.SampleLevel(SamplerPointClamp, float2(roughnessUV + float2(+delta, 0)), lod);
		edges[3] = Roughnessmap.SampleLevel(SamplerPointClamp, float2(roughnessUV + float2(0, -delta)), lod);

		for (i = 0; i < 4; ++i)
			edges[i] = (edges[i] + roughness) * 0.5f;

		for (i = 0; i < 4; ++i)
			output.Edges[i] = max(output.Edges[i] * pow(edges[i], RoughnessModificator), 1);

		for (i = 0; i < 2; ++i)
			output.Inside[i] = max(output.Inside[i] * pow(roughness, RoughnessModificator), 1);
	}

	for (i = 0; i < 4; ++i)
		output.Edges[i] = min(output.Edges[i], MinPatchSize);

	for (i = 0; i < 2; ++i)
		output.Inside[i] = min(output.Inside[i], MinPatchSize);

	float4 color = float4(1, 1, 1, 1);

	if (ShowNodesEnabled)
		color = float4(1, min(lod * 0.5f, 1), 0, 1);

	output.Color = color;
	output.MipMapLevel = 6.0f - log2(max(output.Inside[0], output.Inside[1]));

	return output;
}


[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(NUM_CONTROL_POINTS)]
[patchconstantfunc("HSConstantDefault")]
ControlPoint HSDefault(InputPatch<ControlPoint, NUM_CONTROL_POINTS> input, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	ControlPoint output;

	output.Position = input[pointID].Position;
	output.Size = input[pointID].Size;

	return output;
}


[domain("quad")]
DSOutput DSDefault(HSConstantOutput input, float2 uv : SV_DomainLocation, const OutputPatch<ControlPoint, NUM_CONTROL_POINTS> patch)
{
	DSOutput output;

	ControlPoint cp = patch[0];

	float x = cp.Position.x + cp.Size * uv.x;
	float z = cp.Position.y + cp.Size * (1 - uv.y);

	float2 texCoord = float2(x / TerrainSize, 1.0f - z / TerrainSize);

	bool enabledMipMap = AntiShimmeringEnabled && uv.x != 0 && uv.x != 1 && uv.y != 0 && uv.y != 1;
		
	float lod = enabledMipMap ? input.MipMapLevel : 0;

	float height = Heightmap.SampleLevel(SamplerLinearClamp, texCoord, lod).r;

	float4 position = float4(x, height * Bumpiness, z, 1);

	output.Position = mul(position, WorldViewProjection);
	output.TexCoord = texCoord;
	output.Color = input.Color;
	//output.Culled = input.Culled;

	return output;
}


float4 PSDefault(DSOutput input) : SV_Target
{
	if (ColormapEnabled)
		return Colormap.Sample(SamplerLinearClamp, input.TexCoord);

	if (!LightingEnabled && !SlopeTextureEnabled && !HeightTextureEnabled)
		return 1.0f;

	float2 colormapUV = input.TexCoord * ColormapRepeat;
	float3 normal;

	float diffuse = 1.0f;
	float ambient = 0.2f;

	float4 color = 1.0f;

	if (LightingEnabled || SlopeTextureEnabled)
	{
		normal = normalize(2.0f * Normalmap.Sample(SamplerLinearClamp, input.TexCoord).xzy - 1.0f);
	}

	if (LightingEnabled)
	{
		float3 light = normalize(float3(1, 1, 1));
		diffuse = saturate(dot(normal, light));
	}
	else
	{
		color = 0.0f;
	}

	if (HeightTextureEnabled)
	{
		float height = Heightmap.SampleLevel(SamplerLinearClamp, input.TexCoord, 0).r;
		float4 colorInfo = Infomap.SampleLevel(SamplerPointClamp, input.TexCoord, 0);
		float4 colorFlat0 = Colormaps.Sample(SamplerLinearWrap, float3(colormapUV, colorInfo.x * 255.0f));
		float4 colorFlat1 = Colormaps.Sample(SamplerLinearWrap, float3(colormapUV, colorInfo.y * 255.0f));
		float flatWeight = saturate((height - colorInfo.z) / (colorInfo.w - colorInfo.z));
		color = lerp(colorFlat0, colorFlat1, flatWeight);
	}

	if (SlopeTextureEnabled)
	{
		float4 colorSlope = Slopemap.Sample(SamplerLinearWrap, colormapUV);
		float slopeWeight = saturate(1.0f - (normal.y - SlopeRange.x) / (SlopeRange.y - SlopeRange.x));
		color = lerp(color, colorSlope, slopeWeight);
	}

	if (ShowNodesEnabled)
		color = input.Color;

	color *= saturate(ambient + diffuse) * 0.5f;

	return float4(color.rgb, 1.0f);
}


technique11 Default
{
	pass Pass0
	{
		SetVertexShader(CompileShader(vs_5_0, VSDefault()));
		SetHullShader(CompileShader(hs_5_0, HSDefault()));
		SetDomainShader(CompileShader(ds_5_0, DSDefault()));
		SetPixelShader(CompileShader(ps_5_0, PSDefault()));
	}
}
