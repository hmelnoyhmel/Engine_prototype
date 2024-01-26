#pragma once
#include <wrl.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;

class DirectDevice;
class DirectCommandList;

class DirectRenderTargetManager
{
public:
	DirectRenderTargetManager(DirectDevice& device);

	void InitRenderTarget();
	void OMSetRenderTargets(
		const DirectCommandList& commandList,
		DWORD NumRenderTargets,
		const ComPtr<ID3D12Resource2>* rtvResources,
		const D3D12_RENDER_TARGET_VIEW_DESC* pRTVDescs,
		ComPtr<ID3D12Resource2> dsvResource,
		const D3D12_DEPTH_STENCIL_VIEW_DESC* pDSVDesc) const;

	void RemoveRenderTarget();

	void DirectRenderTargetManager::ClearRenderTargetView(
		const DirectCommandList& commandList,
		ComPtr<ID3D12Resource2> rtvResource,
		const D3D12_RENDER_TARGET_VIEW_DESC* pRTVDesc,
		const FLOAT ColorRGBA[4],
		UINT NumRects,
		const D3D12_RECT* pRects) const;

	void DirectRenderTargetManager::ClearDepthStencilView(
		const DirectCommandList& commandList,
		ComPtr<ID3D12Resource2> dsvResource,
		const D3D12_DEPTH_STENCIL_VIEW_DESC* pDSVDesc,
		D3D12_CLEAR_FLAGS ClearFlags,
		FLOAT Depth,
		UINT8 Stencil,
		UINT NumRects,
		const D3D12_RECT* pRects) const;

private:
	DirectDevice& device; // this breaks explicit creation

	ComPtr<ID3D12DescriptorHeap> rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	UINT rtvIncrementSize;

	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
};