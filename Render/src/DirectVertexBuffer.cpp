#include "DirectVertexBuffer.h"

#include "DirectHelper.h"

DirectVertexBuffer::DirectVertexBuffer(
	DirectDevice& device,
	const std::wstring name,
	UINT elementCount,
	UINT elementStride
)
	: DirectBuffer(device, name, elementCount*elementStride)
{
	// Transfer to base class
	vertexBufferView.BufferLocation = nativeResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = elementCount * elementStride;
	vertexBufferView.StrideInBytes = elementStride;

	count = elementCount;
	stride = elementStride;
	bufferSize = elementCount * elementStride;

}

void DirectVertexBuffer::Attach(ComPtr<ID3D12GraphicsCommandList6> list)
{
	list->IASetVertexBuffers(0, 1, &vertexBufferView);
}
