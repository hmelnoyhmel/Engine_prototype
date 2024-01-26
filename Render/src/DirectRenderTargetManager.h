#pragma once
#include <wrl.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;

class DirectDevice;
class DirectCommandList;

class DirectRenderTargetManager final
{
public:

	void Initialize(ComPtr<ID3D12Device9> CurrentDevice);
	void Dispose();

	void OMSetRenderTargets(
		const ComPtr<ID3D12GraphicsCommandList1> commandList,
		DWORD numRenderTargets,
		const ComPtr<ID3D12Resource2>* rtvResources,
		const D3D12_RENDER_TARGET_VIEW_DESC* rtvDescs,
		ComPtr<ID3D12Resource2> dsvResource,
		const D3D12_DEPTH_STENCIL_VIEW_DESC* dsvDesc) const;	

	void ClearRenderTargetView(
		const ComPtr<ID3D12GraphicsCommandList1> commandList,
		ComPtr<ID3D12Resource2> rtvResource,
		const D3D12_RENDER_TARGET_VIEW_DESC* rtvDesc,
		const FLOAT colorRGBA[],
		UINT numRects,
		const D3D12_RECT* rects) const;

	void ClearDepthStencilView(
		const ComPtr<ID3D12GraphicsCommandList1> commandList,
		ComPtr<ID3D12Resource2> dsvResource,
		const D3D12_DEPTH_STENCIL_VIEW_DESC* dsvDesc,
		D3D12_CLEAR_FLAGS clearFlags,
		FLOAT depth,
		UINT8 stencil,
		UINT numRects,
		const D3D12_RECT* rects) const;

private:
	ComPtr<ID3D12Device1> device = nullptr; // this breaks explicit creation

	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = {};
	UINT rtvIncrementSize = 0;

	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {};
};