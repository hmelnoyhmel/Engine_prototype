#include "DirectTexture.h"

DirectTexture::DirectTexture(
	DirectDevice& device,
	const std::wstring name,
	Vector2 size,
	DXGI_FORMAT format
)
	: DirectResource(device, CD3DX12_RESOURCE_DESC::Tex2D(format, size.x, size.y), name)
{
	
}
