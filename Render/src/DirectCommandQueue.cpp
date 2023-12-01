#include "DirectCommandQueue.h"
#include "DirectDevice.h"
#include "DirectHelper.h"
//#include "DirectCommandList.h"
#include "Enums.h"

DirectCommandQueue::DirectCommandQueue(DirectDevice& device, EQueueType type) :
	r_device{ device }
{
	switch (type)
	{
	case EQueueType::Graphics:
		CreateGraphicsQueue();
		CreateGraphicsAllocator();
		break;
	case EQueueType::Compute:
		CreateComputeQueue();
		CreateComputeAllocator();
		break;
	}

}

void DirectCommandQueue::CreateGraphicsQueue()
{
	D3D12_COMMAND_QUEUE_DESC graphicsQueueDesc = {};
	graphicsQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	graphicsQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(r_device.GetNativeDevice()->CreateCommandQueue(&graphicsQueueDesc, IID_PPV_ARGS(&m_queue)));
}

void DirectCommandQueue::CreateComputeQueue()
{
	D3D12_COMMAND_QUEUE_DESC computeQueueDesc = {};
	computeQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	computeQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(r_device.GetNativeDevice()->CreateCommandQueue(&computeQueueDesc, IID_PPV_ARGS(&m_queue)));
}

void DirectCommandQueue::CreateGraphicsAllocator()
{
	ThrowIfFailed(r_device.GetNativeDevice()->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(m_allocator.GetAddressOf())));
}

void DirectCommandQueue::CreateComputeAllocator()
{
	ThrowIfFailed(r_device.GetNativeDevice()->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_COMPUTE,
		IID_PPV_ARGS(m_allocator.GetAddressOf())));
}

DirectCommandList DirectCommandQueue::GetCommandList()
{
	if (!freeQueue.empty())
	{
		auto list = freeQueue.front();
		freeQueue.pop();
		return std::move(list);
	}

	return std::move(DirectCommandList{ r_device, *this });
}

void DirectCommandQueue::ExecuteCommandList(DirectCommandList& list)
{

	ID3D12CommandList* cmdlists[] = { list.GetNativeList().Get() };
	m_queue->ExecuteCommandLists(_countof(cmdlists), cmdlists);
	
	FlushCmdQueue();

	list.FlushCmdList();

	freeQueue.push(std::move(list));
}

void DirectCommandQueue::FlushCmdQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	r_device.AdvanceFence();

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	ThrowIfFailed(m_queue->Signal(r_device.GetNativeFence().Get(), r_device.GetFenceValue()));

	// Wait until the GPU has completed commands up to this fence point.
	if (r_device.GetNativeFence()->GetCompletedValue() < r_device.GetFenceValue())
	{
		HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		ThrowIfFailed(r_device.GetNativeFence()->SetEventOnCompletion(r_device.GetFenceValue(), eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}
