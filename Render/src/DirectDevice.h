#pragma once
#include "Enums.h"

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

// Linking D3D12 libs
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

// Forward Declarations
class DirectCommandQueue;
class DirectSwapChain;

class DirectDevice
{
private:

	// Native
	ComPtr<IDXGIFactory7> m_factory;
	ComPtr<ID3D12Device9> m_device;


	DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Custom
	std::unordered_map<EQueueType, std::shared_ptr<DirectCommandQueue>> queues;

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

public:
	DirectDevice() = default;
	void CreateDevice();
	inline ComPtr<IDXGIFactory7> GetNativeFactory() const { return m_factory; }
	inline ComPtr<ID3D12Device9> GetNativeDevice() const { return m_device; }

	DirectCommandQueue& GetCommandQueue(EQueueType type);
};