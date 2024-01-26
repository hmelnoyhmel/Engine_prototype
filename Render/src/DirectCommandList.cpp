#include "DirectCommandList.h"
#include "DirectCommandQueue.h"
#include "DirectDevice.h"
#include "DirectHelper.h"
#include "DirectRenderTargetManager.h"

#include <DirectXColors.h>

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

}

void DirectCommandList::FlushCmdList() const
{
	ThrowIfFailed(nativeAllocator->Reset());
	ThrowIfFailed(nativeList.Get()->Reset(nativeAllocator.Get(), nullptr));

	// Debug
	std::wstring text = L"List Flushed.\n";
	OutputDebugString(text.c_str());
}

void DirectCommandList::Test(ComPtr<IDXGISwapChain3> sc, DirectRenderTargetManager& rtManager, int num)
{
	//rtManager.InitRenderTarget();

	sc->GetBuffer(sc->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&backbuffer));

	std::array<ComPtr<ID3D12Resource2>, 1> rtvs;
	rtvs[0] = backbuffer;

	rtManager.OMSetRenderTargets(*this, rtvs.size(), rtvs.data(), nullptr, nullptr, nullptr);

	nativeList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffer.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	switch (num%3) {
	case 0:
		rtManager.ClearRenderTargetView(*this, backbuffer, nullptr, DirectX::Colors::Blue, 0, nullptr);
		break;
	case 1:
		rtManager.ClearRenderTargetView(*this, backbuffer, nullptr, DirectX::Colors::Green, 0, nullptr);
		break;
	case 2:
		rtManager.ClearRenderTargetView(*this, backbuffer, nullptr, DirectX::Colors::DarkViolet, 0, nullptr);
		break;
	}

	nativeList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backbuffer.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
}
