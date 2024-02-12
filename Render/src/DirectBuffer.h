#pragma once
#include "DirectResource.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class DirectResource;
class DirectCommandList;

class DirectBuffer : public DirectResource
{
public:
	// size
	// format

	DirectBuffer(
		DirectDevice& device,
		const std::wstring name,
		UINT size,
		D3D12_HEAP_TYPE type = D3D12_HEAP_TYPE_DEFAULT
	);
	void LoadData(ComPtr<ID3D12Device9> device, ComPtr<ID3D12GraphicsCommandList6> list, void* data);

	//void Attach(ComPtr<ID3D12GraphicsCommandList6> list) override;
protected:
	ComPtr<ID3D12Resource> uploadBuffer;
	ComPtr<ID3DBlob> bufferCPU;
	DWORD bufferSize;
private:
};