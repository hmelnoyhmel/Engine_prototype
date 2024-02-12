#pragma once
#include "DirectResource.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class DirectResource;

class DirectTexture : public DirectResource
{
public:
	// size
	// format

	DirectTexture(
		DirectDevice& device,
		const std::wstring name, 
		Vector2 size, 
		DXGI_FORMAT format
	);

private:
};