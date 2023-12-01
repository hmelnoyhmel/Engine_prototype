#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <memory>
#include <queue>
#include <vector>

#include "DirectCommandList.h"

using Microsoft::WRL::ComPtr;

class DirectDevice;
//class DirectCommandList;
enum class EQueueType;

class DirectCommandQueue
{
private:
	// Native
	ComPtr<ID3D12CommandQueue> m_queue;
	ComPtr<ID3D12CommandAllocator> m_allocator;

	// Custom
	DirectDevice& r_device;

	std::queue<DirectCommandList> freeQueue;

	void CreateGraphicsQueue();
	void CreateGraphicsAllocator();

	void CreateComputeQueue();
	void CreateComputeAllocator();

public:
	DirectCommandQueue(DirectDevice& device, EQueueType type);

	DirectCommandList GetCommandList();
	void ExecuteCommandList(DirectCommandList& list);

	void FlushCmdQueue();

	inline ComPtr<ID3D12CommandQueue> GetNativeQueue() const { return m_queue; }
	inline ComPtr<ID3D12CommandAllocator> GetNativeAllocator() const { return m_allocator; }
};