#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

using Microsoft::WRL::ComPtr;

class DirectDevice;
enum class EQueueType;

class DirectCommandQueue
{
private:
	// Native
	ComPtr<ID3D12CommandQueue> m_queue;

	// Custom
	DirectDevice& r_device;


	void CreateGraphicsQueue();
	void CreateComputeQueue();

public:
	DirectCommandQueue(DirectDevice& device, EQueueType type);
	inline ComPtr<ID3D12CommandQueue> GetNativeQueue() const { return m_queue; }
};