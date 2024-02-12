#pragma once
#include <vector>
#include <wrl.h>

#include "IRenderPipelineAttacher.h"

class DirectDevice;
using namespace Microsoft::WRL;

class DirectPSO : IRenderPipelineAttacher
{
public:

	DirectPSO(DirectDevice& device, ComPtr<ID3DBlob> vsByteCode, ComPtr<ID3DBlob> psByteCode, std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout);

	void Attach(ComPtr<ID3D12GraphicsCommandList6> list) override;

private:
	ComPtr<ID3D12PipelineState> nativePSO;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
};