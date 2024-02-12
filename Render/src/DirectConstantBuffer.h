#pragma once
#include "DirectBuffer.h"

class DirectConstantBuffer : public DirectBuffer
{
public:
	DirectConstantBuffer(
		DirectDevice& device,
		const std::wstring name,
		UINT elementSize,
		UINT elementCount,
		UINT rootSignatureSlot
	);

	~DirectConstantBuffer();

	void Attach(ComPtr<ID3D12GraphicsCommandList6> list) override;

	template<typename T>
	inline void CopyData(int elementIndex, const T& data)
	{
		
		memcpy(&mMappedData[elementIndex * elemSize], &data, sizeof(T));
	}

private:
	BYTE* mMappedData = nullptr;
	UINT elemSize;
	UINT rootSlot;
};