#pragma once
#include "DirectCommandList.h"
#include <wrl.h>
#include <d3d12.h>
#include <queue>

using Microsoft::WRL::ComPtr;

class DirectDevice;
class DirectCommandList;
enum class EQueueType;

class DirectCommandQueue
{
	friend class DirectDevice;

public:
	DirectCommandQueue(DirectDevice& device, EQueueType type);

	void ExecuteCommandList(DirectCommandList& list);

	void FlushCmdQueue() const;

	inline ComPtr<ID3D12CommandQueue> GetNativeQueue() const { return nativeQueue; }
	inline EQueueType GetQueueType() const { return type; }

private:
	ComPtr<ID3D12CommandQueue> nativeQueue;
	EQueueType type;

	DirectDevice& device;

	std::queue<DirectCommandList> freeQueue;

	void CreateGraphicsQueue();
	void CreateGraphicsAllocator();

	void CreateComputeQueue();
	void CreateComputeAllocator();

	DirectCommandList GetCommandList();
};