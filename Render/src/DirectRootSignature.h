#pragma once
#include <d3d12.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

class DirectDevice;

class DirectRootSignature
{
public:
	DirectRootSignature(DirectDevice& device);
	inline ComPtr<ID3D12RootSignature> GetNativeRootSignature() const { return nativeRootSignature; }

private:
	ComPtr<ID3D12RootSignature> nativeRootSignature;
};