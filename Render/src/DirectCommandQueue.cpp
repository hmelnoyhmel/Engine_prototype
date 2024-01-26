#include "DirectCommandQueue.h"
#include "DirectDevice.h"
#include "DirectHelper.h"
#include "Enums.h"

DirectCommandQueue::DirectCommandQueue(DirectDevice& device, EQueueType type) :
	device{ device }, type { type }
{
	switch (type)
	{
	case EQueueType::Graphics:
		CreateGraphicsQueue();
		break;
	case EQueueType::Compute:
		CreateComputeQueue();
		break;
	}


}

void DirectCommandQueue::CreateGraphicsQueue()
{
	D3D12_COMMAND_QUEUE_DESC graphicsQueueDesc = {};
	graphicsQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	graphicsQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(device.GetNativeDevice()->CreateCommandQueue(&graphicsQueueDesc, IID_PPV_ARGS(&nativeQueue)));
}

void DirectCommandQueue::CreateComputeQueue()
{
	D3D12_COMMAND_QUEUE_DESC computeQueueDesc = {};
	computeQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	computeQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(device.GetNativeDevice()->CreateCommandQueue(&computeQueueDesc, IID_PPV_ARGS(&nativeQueue)));
}

DirectCommandList DirectCommandQueue::GetCommandList()
{
	if (!freeQueue.empty())
	{
		auto list = freeQueue.front();
		freeQueue.pop();
		return std::move(list);
	}

	return std::move(DirectCommandList{ device, *this });
}

void DirectCommandQueue::ExecuteCommandList(DirectCommandList& list)
{
	ID3D12CommandList* cmdlists[] = { list.GetNativeList().Get() };
	nativeQueue->ExecuteCommandLists(_countof(cmdlists), cmdlists);

	// Debug
	std::wstring text = L"Command list was executed successfully.\n";
	OutputDebugString(text.c_str());

	FlushCmdQueue();
	list.FlushCmdList();
	freeQueue.push(std::move(list));
}

void DirectCommandQueue::FlushCmdQueue() const
{
	// Advance the fence value to mark commands up to this fence point.
	device.AdvanceFence();

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	ThrowIfFailed(nativeQueue->Signal(device.GetNativeFence().Get(), device.GetFenceValue()));

	// Wait until the GPU has completed commands up to this fence point.
	if (device.GetNativeFence()->GetCompletedValue() < device.GetFenceValue())
	{
		const HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		ThrowIfFailed(device.GetNativeFence()->SetEventOnCompletion(device.GetFenceValue(), eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	// DEBUG
	std::wstring text = L"Queue flushed.\n";
	OutputDebugString(text.c_str());
}
