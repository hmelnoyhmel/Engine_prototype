#include "DirectPSO.h"

#include "d3dx12.h"
#include "DirectDevice.h"
#include "DirectHelper.h"
#include "DirectRootSignature.h"

DirectPSO::DirectPSO(DirectDevice& device, ComPtr<ID3DBlob> vsByteCode, ComPtr<ID3DBlob> psByteCode, std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout)
{
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

    // Shaders Layout
    psoDesc.InputLayout = { inputLayout.data(), (UINT)inputLayout.size() };
    // Root Signature
    psoDesc.pRootSignature = device.GetOrCreateRootSignature()->GetNativeRootSignature().Get();

    // Compiled Shaders
    psoDesc.VS =
    {
        reinterpret_cast<BYTE*>(vsByteCode->GetBufferPointer()),
        vsByteCode->GetBufferSize()
    };
    psoDesc.PS =
    {
        reinterpret_cast<BYTE*>(psByteCode->GetBufferPointer()),
        psByteCode->GetBufferSize()
    };

    /*
    auto ds = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    ds.DepthEnable = false;
    ds.StencilEnable = false;
    */

    auto rasterizer = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    rasterizer.CullMode = D3D12_CULL_MODE_FRONT;
    rasterizer.FillMode = D3D12_FILL_MODE_SOLID;
    psoDesc.RasterizerState = rasterizer;
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // SwapChain? HARDCODED
    psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;  // SwapChain? HARDCODED
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); //ds;
    // MSAA section?
    psoDesc.SampleDesc.Count = 1;
    psoDesc.SampleDesc.Quality = 0;

    ThrowIfFailed(device.GetNativeDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&nativePSO)));
}


void DirectPSO::Attach(ComPtr<ID3D12GraphicsCommandList6> list)
{
	list->SetPipelineState(nativePSO.Get());
}
