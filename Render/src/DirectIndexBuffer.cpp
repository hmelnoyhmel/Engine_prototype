#include "DirectIndexBuffer.h"

#include "DirectHelper.h"

DirectIndexBuffer::DirectIndexBuffer(
	DirectDevice& device,
	const std::wstring name,
	UINT elementCount
)
	: DirectBuffer(device, name, elementCount * sizeof(std::uint16_t))
{
	indexBufferView.BufferLocation = nativeResource->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	indexBufferView.SizeInBytes = elementCount * sizeof(std::uint16_t);

	count = elementCount;
	bufferSize = elementCount * sizeof(std::uint16_t);

	ThrowIfFailed(D3DCreateBlob(bufferSize, &bufferCPU));
}

void DirectIndexBuffer::Attach(ComPtr<ID3D12GraphicsCommandList6> list)
{
	list->IASetIndexBuffer(&indexBufferView);
}
