#pragma once
#include "DirectBuffer.h"

class DirectIndexBuffer : public DirectBuffer
{
public:
	// size

	DirectIndexBuffer(
		DirectDevice& device,
		const std::wstring name,
		UINT elementCount
	);

	void Attach(ComPtr<ID3D12GraphicsCommandList6> list) override;

private:
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	UINT count;
};