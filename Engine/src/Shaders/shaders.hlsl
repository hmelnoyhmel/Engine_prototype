//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

struct objectData
{
	float4x4 World;
};

struct cameraData
{
	float4x4 View;
	float4x4 InvView;
	float4x4 Proj;
	float4x4 InvProj;
	float4x4 ViewProj;
	float4x4 InvViewProj;
};

ConstantBuffer<objectData> objectBuffer : register(b0);
ConstantBuffer<cameraData> cameraBuffer : register(b1);

struct VertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
	float3 PosW : POSITION0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout = (VertexOut)0.0f;

	float4 posW = mul(float4(vin.PosL, 1.0f), objectBuffer.World);
	vout.PosW = posW.xyz;
	vout.PosH = mul(posW, cameraBuffer.ViewProj);


	// Just pass vertex color into the pixel shader.
	vout.Color = vin.Color;

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}