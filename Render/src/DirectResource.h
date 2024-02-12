#pragma once
#include <d3d12.h>
#include <memory>
#include <string>
#include <wrl.h>
#include "d3dx12.h"
#include "IRenderPipelineAttacher.h"

using Microsoft::WRL::ComPtr;

class DirectDevice;
class DirectCommandList;

class DirectResource : public IRenderPipelineAttacher
{
public:
	// size
	// format
	// usage SRV|UAV|RTV

	DirectResource(
		DirectDevice& device,
		const D3D12_RESOURCE_DESC& resourceDesc, 
		const std::wstring name = L"",
		D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_DEFAULT,
		D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE,
		D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_COMMON
	);


	inline void SetName(std::wstring name) { resourceName = name; }
	inline std::wstring GetName() { return resourceName; }

	inline ComPtr<ID3D12Resource2> GetNativeResource() const{ return nativeResource; }

	void Attach(ComPtr<ID3D12GraphicsCommandList6> list) override = 0;

protected:
	friend DirectCommandList;
	DirectDevice& device;
	ComPtr<ID3D12Resource2> nativeResource;
	std::wstring resourceName;
};