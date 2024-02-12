#include "DirectBuffer.h"
#include "DirectCommandList.h"
#include "DirectHelper.h"

DirectBuffer::DirectBuffer(
	DirectDevice& device,
	const std::wstring name,
	UINT size,
	D3D12_HEAP_TYPE type
)
	: DirectResource(device, CD3DX12_RESOURCE_DESC::Buffer(size), name, type)
{

	ThrowIfFailed(D3DCreateBlob(size, &bufferCPU));
	bufferSize = size;
}


void DirectBuffer::LoadData(ComPtr<ID3D12Device9> device, ComPtr<ID3D12GraphicsCommandList6> list, void* data)
{
	CopyMemory(bufferCPU->GetBufferPointer(), data, bufferSize);
	UploadBufferData(device.Get(), nativeResource, list.Get(), data, bufferSize, uploadBuffer);
}

