#include "DirectCommandList.h"
#include "DirectCommandQueue.h"
#include "DirectDevice.h"
#include "DirectHelper.h"
#include "DirectRenderTargetManager.h"

#include <DirectXColors.h>

#include "DirectResource.h"
#include "DirectRootSignature.h"

static DirectRenderTargetManager RenderHelper;

DirectCommandList::DirectCommandList(DirectDevice& device, DirectCommandQueue& queue) :
	device { device }, queue { queue }
{
	switch (queue.GetQueueType())
	{
	case EQueueType::Graphics:
		// CreateCommandList1 creates closed list
		ThrowIfFailed(device.GetNativeDevice()->CreateCommandList1(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			D3D12_COMMAND_LIST_FLAG_NONE,
			IID_PPV_ARGS(&nativeList)));
		ThrowIfFailed(device.GetNativeDevice()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(nativeAllocator.GetAddressOf())));
		break;
	case EQueueType::Compute:
		// CreateCommandList1 creates closed list
		ThrowIfFailed(device.GetNativeDevice()->CreateCommandList1(
			0,
			D3D12_COMMAND_LIST_TYPE_COMPUTE,
			D3D12_COMMAND_LIST_FLAG_NONE,
			IID_PPV_ARGS(&nativeList)));
		ThrowIfFailed(device.GetNativeDevice()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_COMPUTE,
			IID_PPV_ARGS(nativeAllocator.GetAddressOf())));
		break;
	}

	// First close, then reset
	FlushCmdList();

	// Debug
	std::wstring text = L"Command list was created successfully.\n";
	OutputDebugString(text.c_str());

	RenderHelper.Initialize(device.GetNativeDevice());
}

void DirectCommandList::SetRenderTargets(const std::vector<ComPtr<ID3D12Resource2>>& rtvResources,
	const D3D12_RENDER_TARGET_VIEW_DESC* pRTVDescs, ComPtr<ID3D12Resource2> dsvResource,
	const D3D12_DEPTH_STENCIL_VIEW_DESC* pDSVDesc)
{
	for (auto&& rtv : rtvResources)
		nativeList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rtv.Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	RenderHelper.OMSetRenderTargets(nativeList, rtvResources.size(), rtvResources.data(), pRTVDescs, dsvResource, pDSVDesc);
}

void DirectCommandList::ClearRenderTarget(const ComPtr<ID3D12Resource2>& rtvResource,
	const D3D12_RENDER_TARGET_VIEW_DESC* pRTVDesc, const FLOAT ColorRGBA[4], const UINT NumRects,
	const D3D12_RECT* pRects) const
{
	RenderHelper.ClearRenderTargetView(nativeList, rtvResource, pRTVDesc, ColorRGBA, NumRects, pRects);
}

void DirectCommandList::ClearDepthStencil(const ComPtr<ID3D12Resource2>& dsvResource,
                                          const D3D12_DEPTH_STENCIL_VIEW_DESC* pDSVDesc, const D3D12_CLEAR_FLAGS ClearFlags, const FLOAT Depth, const UINT8 Stencil,
                                          const UINT NumRects, const D3D12_RECT* pRects) const
{
	RenderHelper.ClearDepthStencilView(nativeList,dsvResource, pDSVDesc, ClearFlags, Depth, Stencil, NumRects, pRects);
}

void DirectCommandList::FlushCmdList() const
{
	ThrowIfFailed(nativeAllocator->Reset());
	ThrowIfFailed(nativeList.Get()->Reset(nativeAllocator.Get(), nullptr));

	// Debug
	const std::wstring text = L"List Flushed.\n";
	OutputDebugString(text.c_str());
}

void DirectCommandList::Test(ComPtr<ID3D12Resource2>& backbuffer, const FLOAT ColorRGBA[4])
{
	nativeList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffer.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	ClearRenderTarget(backbuffer.Get(), nullptr, ColorRGBA, 0, nullptr);
	//ClearDepthStencil(backbuffer.Get(), nullptr, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

}

void DirectCommandList::Close() const
{
	nativeList->Close();
}

void DirectCommandList::SetRootSignature(DirectRootSignature& rootSignature)
{
	auto rootSig = rootSignature.GetNativeRootSignature();
	nativeList->SetGraphicsRootSignature(rootSig.Get());
}

void DirectCommandList::SetPresentState(ComPtr<ID3D12Resource2>& backbuffer)
{
	nativeList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffer.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}

void DirectCommandList::SetVieports(const D3D12_VIEWPORT& screenViewport)
{
	nativeList->RSSetViewports(1, &screenViewport);
}

void DirectCommandList::SetScissorRects(const D3D12_RECT& scissorRect)
{
	nativeList->RSSetScissorRects(1, &scissorRect);
}

/*
void DirectCommandList::SetResourceBarrier(DirectResource& resource, )
{
	nativeList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource.GetNativeResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}
*/