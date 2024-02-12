#include "DirectConstantBuffer.h"

#include "DirectHelper.h"

DirectConstantBuffer::DirectConstantBuffer(
	DirectDevice& device, 
	const std::wstring name, 
	UINT elementSize, 
	UINT elementCount,
	UINT rootSignatureSlot
) : DirectBuffer(device, name, elementSize*elementCount, D3D12_HEAP_TYPE_UPLOAD)
{
	elemSize = elementSize;
	rootSlot = rootSignatureSlot;

	ThrowIfFailed(nativeResource->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));
}

DirectConstantBuffer::~DirectConstantBuffer()
{
	if (nativeResource != nullptr)
		nativeResource->Unmap(0, nullptr);
	mMappedData = nullptr;
}

/*
template<typename T>
void DirectConstantBuffer::CopyData(int elementIndex, const T& data)
{
	memcpy(&mMappedData[elementIndex * elemSize], &data, sizeof(T));
}
*/

void DirectConstantBuffer::Attach(ComPtr<ID3D12GraphicsCommandList6> list)
{
	list->SetGraphicsRootConstantBufferView(rootSlot, nativeResource->GetGPUVirtualAddress());
}
