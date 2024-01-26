#pragma once
#include <wrl.h>
#include <array>
#include <d3d12.h>
#include <d3dx12.h>

// Testing feature
#include <vector>

#include "DirectSwapChain.h"

using Microsoft::WRL::ComPtr;

class DirectDevice;
class DirectCommandQueue;
class DirectSwapChain;

class DirectCommandList
{
public:
	DirectCommandList(DirectDevice& device, DirectCommandQueue& queue);

	void SetRenderTargets(
		const std::vector<ComPtr<ID3D12Resource2>>& rtvResources,
		const D3D12_RENDER_TARGET_VIEW_DESC* pRTVDescs = nullptr, ComPtr<ID3D12Resource2> dsvResource = nullptr,
		const D3D12_DEPTH_STENCIL_VIEW_DESC* pDSVDesc = nullptr);


	void ClearRenderTarget(
		const ComPtr<ID3D12Resource2>& rtvResource,
		const D3D12_RENDER_TARGET_VIEW_DESC* pRTVDesc,
		const FLOAT ColorRGBA[],
		const UINT NumRects,
		const D3D12_RECT* pRects) const;

	void ClearDepthStencil(
		const ComPtr<ID3D12Resource2>& dsvResource,
		const D3D12_DEPTH_STENCIL_VIEW_DESC* pDSVDesc,
		D3D12_CLEAR_FLAGS ClearFlags,
		FLOAT Depth,
		UINT8 Stencil,
		UINT NumRects,
		const D3D12_RECT* pRects) const;
	

	void FlushCmdList() const;

	void Test(ComPtr<ID3D12Resource2>& backbuffer, const FLOAT ColorRGBA[]);
	void Close() const;

private:
	friend DirectCommandQueue;
	inline ComPtr<ID3D12GraphicsCommandList6> GetNativeList() const { return nativeList; }
	inline ComPtr<ID3D12CommandAllocator> GetNativeAllocator() const { return nativeAllocator; }

	ComPtr<ID3D12GraphicsCommandList6> nativeList;
	ComPtr<ID3D12CommandAllocator> nativeAllocator;

	DirectDevice& device;
	DirectCommandQueue& queue;


	//ComPtr<ID3D12DescriptorHeap> rtvHeap;
	//UINT rtvDescriptorSize = 0;
};
