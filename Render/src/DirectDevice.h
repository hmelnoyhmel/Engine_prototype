#pragma once
#include "Enums.h"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <unordered_map>
#include <memory>

// Linking D3D12 libs
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

class DirectCommandList;
class DirectCommandQueue;
class DirectSwapChain;
class DirectRenderTargetManager;

class DirectDevice
{
public:
	DirectDevice() = default;
	void CreateDevice();

	inline ComPtr<IDXGIFactory7> GetNativeFactory() const { return nativeFactory; }
	inline ComPtr<ID3D12Device9> GetNativeDevice() const { return nativeDevice; }

	inline ComPtr<ID3D12Fence> GetNativeFence() const { return nativeFence; }
	inline UINT64 GetFenceValue() const { return nativeFenceValue; }
	inline HANDLE GetFenceEvent() const { return nativeFenceEvent; }
	inline void AdvanceFence() { ++nativeFenceValue; }

	DirectCommandQueue& GetCommandQueue(EQueueType type);
	DirectCommandList GetCommandList(EQueueType type);
	DirectRenderTargetManager GetRenderTargetManager();


private:

	// Native
	ComPtr<IDXGIFactory7> nativeFactory;
	ComPtr<ID3D12Device9> nativeDevice;
	ComPtr<ID3D12Fence> nativeFence;
	UINT64 nativeFenceValue = 0;
	HANDLE nativeFenceEvent = nullptr;

	std::shared_ptr<DirectRenderTargetManager> renderTargetManager = nullptr;

	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Custom
	std::unordered_map<EQueueType, std::shared_ptr<DirectCommandQueue>> queues;

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);


};