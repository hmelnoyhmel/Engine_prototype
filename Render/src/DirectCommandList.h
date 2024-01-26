#pragma once
#include <wrl.h>
#include <array>
#include <d3d12.h>
#include <d3dx12.h>

// Testing feature
#include "DirectSwapChain.h"

using Microsoft::WRL::ComPtr;

class DirectDevice;
class DirectCommandQueue;
class DirectSwapChain;
class DirectRenderTargetManager;

class DirectCommandList
{
public:
	DirectCommandList(DirectDevice& device, DirectCommandQueue& queue);

	inline ComPtr<ID3D12GraphicsCommandList6> GetNativeList() const { return nativeList; }
	inline ComPtr<ID3D12CommandAllocator> GetNativeAllocator() const { return nativeAllocator; }

	void FlushCmdList() const;

	void Test(ComPtr<IDXGISwapChain3> sc, DirectRenderTargetManager& rtManager, int num);

private:
	ComPtr<ID3D12GraphicsCommandList6> nativeList;
	ComPtr<ID3D12CommandAllocator> nativeAllocator;

	DirectDevice& device;
	DirectCommandQueue& queue;


	//ComPtr<ID3D12DescriptorHeap> rtvHeap;
	//UINT rtvDescriptorSize = 0;
	ComPtr<ID3D12Resource2> backbuffer;
};
