#pragma once
#include "DirectBuffer.h"

class DirectVertexBuffer : public DirectBuffer
{
public:
	// size

	DirectVertexBuffer(
		DirectDevice& device,
		const std::wstring name,
		UINT elementSize,
		UINT elementStride
	);

	void CreateVBV();

	void Attach(ComPtr<ID3D12GraphicsCommandList6> list) override;

private:
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	UINT count;
	UINT stride;
};