#include "DirectRenderTargetManager.h"

#include "DirectDevice.h"
#include "DirectHelper.h"
#include "DirectCommandList.h"

DirectRenderTargetManager::DirectRenderTargetManager(DirectDevice& device) :
    device{ device }
{
    InitRenderTarget();
}

void DirectRenderTargetManager::InitRenderTarget()
{
    rtvIncrementSize = device.GetNativeDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
    rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvDesc.NumDescriptors = D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT;
    ThrowIfFailed(device.GetNativeDevice()->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&rtvHeap)));

	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_DESCRIPTOR_HEAP_DESC dsvDesc = {};
	dsvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvDesc.NumDescriptors = 1;
    ThrowIfFailed(device.GetNativeDevice()->CreateDescriptorHeap(&dsvDesc, IID_PPV_ARGS(&dsvHeap)));

    dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

}

void DirectRenderTargetManager::OMSetRenderTargets(
    const DirectCommandList& commandList,
    DWORD numRenderTargets,
    const ComPtr<ID3D12Resource2>* rtvResources,
    const D3D12_RENDER_TARGET_VIEW_DESC* rtvDescs,
    ComPtr<ID3D12Resource2> dsvResource,
    const D3D12_DEPTH_STENCIL_VIEW_DESC* dsvDesc) const
{

    D3D12_CPU_DESCRIPTOR_HANDLE currentRtvHandle = rtvHandle;
    for (DWORD i = 0; i < numRenderTargets; ++i)
    {
        device.GetNativeDevice()->CreateRenderTargetView(
            rtvResources[i].Get(),
            rtvDescs ? rtvDescs + i : nullptr,
            currentRtvHandle);
        currentRtvHandle.ptr += rtvIncrementSize;
    }

    const D3D12_CPU_DESCRIPTOR_HANDLE* DSVHandle = nullptr;
    if (dsvResource)
    {
        device.GetNativeDevice()->CreateDepthStencilView(dsvResource.Get(), dsvDesc, dsvHandle);
        DSVHandle = &dsvHandle;
    }

    // make this a separate method in DirectCommandList
    commandList.GetNativeList()->OMSetRenderTargets(numRenderTargets, &rtvHandle, TRUE, DSVHandle);
}

void DirectRenderTargetManager::RemoveRenderTarget()
{
    dsvHandle = {};
    rtvHandle = {};

    if (dsvHeap)
    {
        dsvHeap->Release();
    }
    if (rtvHeap)
    {
        rtvHeap->Release();
    }
}

void DirectRenderTargetManager::ClearRenderTargetView(
    const DirectCommandList& commandList,
    ComPtr<ID3D12Resource2> rtvResource,
    const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc,
    const FLOAT colorRGBA[4],
    UINT numRects,
    const D3D12_RECT* rects) const
{
    device.GetNativeDevice()->CreateRenderTargetView(rtvResource.Get(), rtvDesc, rtvHandle);

    // made through GetNativeList() because user won't call list's ClearRTV, but instead will call this one
    commandList.GetNativeList()->ClearRenderTargetView(rtvHandle, colorRGBA, numRects, rects);
}

void DirectRenderTargetManager::ClearDepthStencilView(
    const DirectCommandList& commandList,
    ComPtr<ID3D12Resource2> dsvResource,
    const D3D12_DEPTH_STENCIL_VIEW_DESC* dsvDesc,
    D3D12_CLEAR_FLAGS clearFlags,
    FLOAT depth,
    UINT8 stencil,
    UINT numRects,
    const D3D12_RECT* rects) const
{
    device.GetNativeDevice()->CreateDepthStencilView(dsvResource.Get(), dsvDesc, dsvHandle);

    // made through GetNativeList() because user won't call list's ClearDSV, but instead will call this one
    commandList.GetNativeList()->ClearDepthStencilView(dsvHandle, clearFlags, depth, stencil, numRects, rects);
}


/*
{
    // When initializing the entire engine:
    DirectRenderTarget renderTarget(device);

    // While rendering a frame:
    renderTarget.ClearRenderTargetView(cmdList, ComPtrRenderTargetTexture, NULL, Color, 0, NULL);
    renderTarget.OMSetRenderTargets(cmdList, 1, &pRenderTargetTexture, NULL, NULL, NULL);
    // Recording commands that render into that render target...
    pCommandList->Close();
    pQueue->ExecuteCommandLists(1, &pCommandList);
    pQueue->Signal(pFence, FrameFenceValue);

    // Sometime later, after N frames, make sure that command list finished execution.
    pFence->SetEventOnCompletion(FrameFenceValue, hEvent);
    WaitForSingleObject(hEvent, INFINITE);
}
*/

