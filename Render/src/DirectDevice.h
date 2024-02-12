#pragma once
#include "Enums.h"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <unordered_map>
#include <memory>
#include <string>

// Linking D3D12 libs
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


using Microsoft::WRL::ComPtr;

class DirectCommandList;
class DirectCommandQueue;
class DirectSwapChain;
class DirectResource;
class DirectRootSignature;

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

	bool TryGetResource(_In_ EResourceType type, _In_ std::string name, _Out_ DirectResource*& output);

	std::shared_ptr<DirectRootSignature> GetOrCreateRootSignature();

private:

	// Native
	ComPtr<IDXGIFactory7> nativeFactory;
	ComPtr<ID3D12Device9> nativeDevice;
	ComPtr<ID3D12Fence> nativeFence;
	UINT64 nativeFenceValue = 0;
	HANDLE nativeFenceEvent = nullptr;

	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Custom
	std::unordered_map<EQueueType, std::shared_ptr<DirectCommandQueue>> queues;

	// CreateResource() method
	// inside will call cmdList and execute it immediately
	// resource creation itself is made in cmdList
	std::unordered_map<std::string, DirectResource&> resources;

	std::shared_ptr<DirectRootSignature> rootSignature;

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);


};